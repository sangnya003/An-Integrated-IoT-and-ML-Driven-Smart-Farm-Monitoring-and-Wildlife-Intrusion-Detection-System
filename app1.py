from flask import Flask, render_template, request, redirect, url_for
import os
from werkzeug.utils import secure_filename
from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing import image
import numpy as np
import serial

app = Flask(__name__)

# Configuration
app.config['UPLOAD_FOLDER'] = 'static/images'
app.config['ALLOWED_EXTENSIONS'] = {'png', 'jpg', 'jpeg'}
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024  # 16 MB

# Load the trained model (MobileNetV2)
model = load_model('balanced_model.h5')

# Class labels (must match training order)
class_labels = ['Elephant', 'Monkey', 'Wild Boar']

#  Serial communication setup
serial_port = "COM3"
baud_rate = 9600
serial_messages = {
    "Elephant": "D1",
    "Monkey": "D2",
    "Wild Boar": "D3"
}

# Animal info for result page
animal_info = {
    'Elephant': {
        'description': 'Elephants are the largest land animals...',
        'habitat': 'Savannas, forests, deserts in Africa and Asia',
        'diet': 'Herbivorous (grasses, leaves, etc.)',
        'conservation': 'African elephants: Vulnerable, Asian: Endangered'
    },
    'Monkey': {
        'description': 'Monkeys are primates known for their intelligence...',
        'habitat': 'Rainforests, savannas, mountainous regions',
        'diet': 'Omnivorous (fruits, insects, etc.)',
        'conservation': 'Varies (Least Concern to Endangered)'
    },
    'Wild Boar': {
        'description': 'Wild boars are sturdy animals with tusks...',
        'habitat': 'Forests, grasslands, wetlands in Europe, Asia',
        'diet': 'Omnivorous (roots, berries, small animals)',
        'conservation': 'Least Concern'
    }
}

#  Validate image extension
def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in app.config['ALLOWED_EXTENSIONS']

#  Send serial signal
def send_serial_message(prediction):
    try:
        ser = serial.Serial(serial_port, baud_rate, timeout=1)
        message = serial_messages.get(prediction, "Unknown")
        ser.write(message.encode())
        ser.close()
        print(f" Sent to serial: {message}")
    except serial.SerialException as e:
        print(" Serial communication error:", e)

#  Predict uploaded image
def predict_image(image_path):
    img = image.load_img(image_path, target_size=(128, 128))  # ✅ Match training size
    img_array = image.img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)
    img_array = img_array / 255.0  #  Normalize

    prediction = model.predict(img_array)[0]
    print("🔍 Raw prediction:", prediction)

    predicted_index = np.argmax(prediction)
    predicted_class = class_labels[predicted_index]
    confidence = round(100 * prediction[predicted_index], 2)

    return predicted_class, confidence

#  Web routes
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/about')
def about():
    return render_template('about.html')

@app.route('/upload', methods=['POST'])
def upload_file():
    if 'file' not in request.files:
        return redirect(request.url)

    file = request.files['file']
    if file.filename == '':
        return redirect(request.url)

    if file and allowed_file(file.filename):
        filename = secure_filename(file.filename)
        filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
        file.save(filepath)

        predicted_class, confidence = predict_image(filepath)
        send_serial_message(predicted_class)
        info = animal_info.get(predicted_class, {})

        return render_template('result.html',
                               image_path=filepath,
                               prediction=predicted_class,
                               confidence=confidence,
                               info=info)

    return redirect(request.url)

# Run the app
if __name__ == '__main__':
    os.makedirs(app.config['UPLOAD_FOLDER'], exist_ok=True)
    app.run(debug=True)
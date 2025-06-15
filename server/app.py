# flask_server.py

from flask import Flask, request, jsonify
from ultralytics import YOLO
import cv2
import numpy as np
import os
from datetime import datetime

app = Flask(__name__)

# Load trained model
model = YOLO("C:/Users/Hariz/Downloads/hackathon x entreprenuership/sketch_jun13b/best.pt")  

@app.route('/upload', methods=['POST'])
def classify_image():
    if 'image' not in request.files:
        return jsonify({'error': 'No image uploaded'}), 400

    file = request.files['image']
    img_bytes = np.frombuffer(file.read(), np.uint8)
    image = cv2.imdecode(img_bytes, cv2.IMREAD_COLOR)

    if image is None:
        return jsonify({'error': 'Failed to decode image'}), 400

    # Save image with timestamp (optional for debugging)
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    os.makedirs("received", exist_ok=True)
    cv2.imwrite(f"received/image_{timestamp}.jpg", image)

    # Run inference
    results = model(image)

    # Parse results
    result = results[0]
    boxes = result.boxes
    class_ids = boxes.cls.cpu().numpy().astype(int)
    confidences = boxes.conf.cpu().numpy()

    if len(class_ids) == 0:
        return jsonify({'message': 'No object detected'}), 200

    predictions = []
    for cls_id, conf in zip(class_ids, confidences):
        label = model.names[cls_id]
        predictions.append({'label': label, 'confidence': float(conf)})

    return jsonify({'predictions': predictions}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

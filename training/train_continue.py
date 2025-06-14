from ultralytics import YOLO

# Load the model from a previous checkpoint (e.g., best.pt or last.pt)
model = YOLO('runs/detect/train/weights/best.pt')  # or 'last.pt'

# Continue training
model.train(
    data='path/to/data.yaml',   # same YAML you used before
    resume=True                 # this is important!
)

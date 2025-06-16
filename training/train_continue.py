import os
from ultralytics import YOLO

if __name__ == "__main__":
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))
    CHECKPOINT_PATH = os.path.join(BASE_DIR, 'runs', 'detect', 'train', 'weights', 'best.pt')
    DATA_PATH = os.path.join(BASE_DIR, 'data.yaml')
    model = YOLO(CHECKPOINT_PATH) 

    # Continue training
    model.train(
        data=DATA_PATH,   # same YAML variable (no quotes!)
        resume=True       # this is important!
    )
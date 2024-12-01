from flask import Flask, render_template, Response, request, jsonify
import cv2
import urllib.request
import numpy as np
from pydrive.drive import GoogleDrive
from pydrive.auth import GoogleAuth
import os

app = Flask(__name__)

# Camera URL
url = 'http://192.168.22.15/capture'

# Authenticate with Google Drive
gauth = GoogleAuth()

# Try to load saved credentials
gauth.LoadCredentialsFile("credentials.json")

if gauth.credentials is None:
    # Authenticate if no credentials file is found
    gauth.LocalWebserverAuth()
elif gauth.access_token_expired:
    # Refresh the credentials if expired
    gauth.Refresh()
else:
    # Initialize the saved credentials
    gauth.Authorize()

# Save the current credentials to a file for reuse
gauth.SaveCredentialsFile("credentials.json")

drive = GoogleDrive(gauth)

# Folder ID in Google Drive
folder_id = "1WgCv__gJb02bAJ7D23oaBiE3ClhnJtTj"  # Replace with your folder ID
file_name = "1.png"  # File name to save on Google Drive

def upload_to_drive(local_file, drive_file_name, folder_id):
    """Upload or overwrite a file in Google Drive."""
    query = f"'{folder_id}' in parents and title='{drive_file_name}' and trashed=false"
    file_list = drive.ListFile({'q': query}).GetList()

    if file_list:
        file_to_update = file_list[0]
        file_to_update.SetContentFile(local_file)
        file_to_update.Upload()
        print(f"File '{drive_file_name}' was updated successfully.")
    else:
        new_file = drive.CreateFile({'title': drive_file_name, 'parents': [{'id': folder_id}]})
        new_file.SetContentFile(local_file)
        new_file.Upload()
        print(f"File '{drive_file_name}' was created successfully.")

def capture_frame():
    """Capture a frame from the live feed."""
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    frame = cv2.imdecode(imgnp, -1)
    return frame

@app.route('/')
def index():
    """Homepage to display the live feed."""
    return render_template('index.html')

@app.route('/capture', methods=['POST'])
def capture():
    """Endpoint to capture an image and upload it to Google Drive."""
    print('aaaa')
    frame = capture_frame()
    local_file_path = file_name
    cv2.imwrite(local_file_path, frame)
    upload_to_drive(local_file_path, file_name, folder_id)
    os.remove(local_file_path)  # Clean up the local file
    return jsonify({"message": "Image captured and uploaded to Google Drive!"})

def generate_frames():
    """Generate video frames for live feed."""
    while True:
        frame = capture_frame()
        _, buffer = cv2.imencode('.jpg', frame)
        frame_data = buffer.tobytes()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame_data + b'\r\n')

@app.route('/video_feed')
def video_feed():
    """Video feed route."""
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
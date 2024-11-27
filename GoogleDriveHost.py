import cv2
import urllib.request
import numpy as np
from pydrive.drive import GoogleDrive
from pydrive.auth import GoogleAuth

# Camera URL
url = 'http://192.168.88.15/capture'
cv2.namedWindow("live transmission", cv2.WINDOW_AUTOSIZE)

# Authenticate with Google Drive
gauth = GoogleAuth()
gauth.LocalWebserverAuth()
drive = GoogleDrive(gauth)

# Folder ID in Google Drive
folder_id = "1WgCv__gJb02bAJ7D23oaBiE3ClhnJtTj"  # Replace with your folder ID
file_name = "1.png"  # The file name to overwrite

def upload_to_drive(local_file, drive_file_name, folder_id):
    """Upload or overwrite a file in Google Drive."""
    # Search for the file in the specified folder
    query = f"'{folder_id}' in parents and title='{drive_file_name}' and trashed=false"
    file_list = drive.ListFile({'q': query}).GetList()

    if file_list:
        # If file exists, overwrite it
        file_to_update = file_list[0]
        file_to_update.SetContentFile(local_file)
        file_to_update.Upload()
        print(f"File '{drive_file_name}' was updated successfully.")
    else:
        # If file does not exist, create a new one
        new_file = drive.CreateFile({'title': drive_file_name, 'parents': [{'id': folder_id}]})
        new_file.SetContentFile(local_file)
        new_file.Upload()
        print(f"File '{drive_file_name}' was created successfully.")

while True:
    # Capture the live feed
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    frame = cv2.imdecode(imgnp, -1)

    cv2.imshow("live transmission", frame)

    key = cv2.waitKey(5)

    if key == ord('k'):  # Save and upload image when 'k' is pressed
        print("Capturing and uploading image...")
        local_file_path = file_name  # Save image as '1.png'
        cv2.imwrite(local_file_path, frame)
        print(f"Image saved locally as: {local_file_path}")
        upload_to_drive(local_file_path, file_name, folder_id)

    if key == ord('q'):  # Quit when 'q' is pressed
        break

cv2.destroyAllWindows()

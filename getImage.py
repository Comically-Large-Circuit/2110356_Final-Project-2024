from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive

# Authenticate with Google Drive
gauth = GoogleAuth()
gauth.LocalWebserverAuth()  # This will open a web browser for authentication
drive = GoogleDrive(gauth)

# Specify the folder ID
folder_id = "1WgCv__gJb02bAJ7D23oaBiE3ClhnJtTj"  # Replace with your folder's ID

# Query files in the specified folder
file_list = drive.ListFile({'q': f"'{folder_id}' in parents and trashed=false"}).GetList()

print(f"Files in folder {folder_id}:")
for file in file_list:
    print(f"File Name: {file['title']} | File ID: {file['id']}")
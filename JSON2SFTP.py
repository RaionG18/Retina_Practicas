import pysftp #https://pysftp.readthedocs.io/en/release_0.2.9/
import glob
import os.path

def upload_file(file_path):

    private_key = "~/.ssh/your-key.pem"  # can use password keyword in Connection instead
    srv = pysftp.Connection(host="your-host", username="user-name", private_key=private_key)
    srv.chdir('/var/web/public_files/media/uploads')  # change directory on remote server
    srv.put(file_path)  # To download a file, replace put with get
    srv.close()  # Close connection


while True:
    #Identificar el ultimo archivo creado
    path = r'C:\Users\emili\OneDrive\Documents\GitHub\Retina_Practicas'
    folder_path = r'{}'.format(path)
    file_type = '\*json'
    files = glob.glob(folder_path + file_type)
    max_file = max(files, key=os.path.getctime)

    #Enviar el archivo por SFTP
    upload_file(max_file)


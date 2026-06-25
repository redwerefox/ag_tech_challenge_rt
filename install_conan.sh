# install conan by creating an venv and installing via pip if missing
# then activate the venv

python3 --version
pip3 --version

#update pip to python3 installed version
python3 -m pip install --upgrade pip

python3 -m venv venv-conan
echo "Conan virtual environment created"
source venv-conan/Scripts/activate
echo "Conan virtual environment activated"
pip install conan
echo "Conan installed in virtual environment"



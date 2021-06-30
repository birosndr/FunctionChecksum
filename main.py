import settings as stt

import argparse
import os

from P4 import P4, P4Exception

class PerforceManager:

	__instance = None
	@staticmethod 
	def get_instance():
		if PerforceManager.__instance == None:
			PerforceManager ()

		return PerforceManager.__instance

	def __init__(self):
		if PerforceManager.__instance != None:
			raise Exception("This class is a singleton!")
		else:
			self.__initialize_credentials ()
			PerforceManager.__instance = self


	def __initialize_credentials (self):
		self._p4 = P4()
		self._p4.port = stt.PORT
		self._p4.user = stt.USER
		self._p4.password = stt.PASSWORD
		self._p4.client = stt.CLIENT
		self._p4.charset = stt.CHARSET

	def connecto_to_server (self):
		self._p4.connect()

	def disconnect_from_server (self):
		self._p4.disconnect()

	def get_changelist_info (self, changelist):
		return self._p4.run( "describe", "-s", changelist)

	def dump_file_content (self, file_path, actual_revision):
		file_name = file_path[22:-4].replace ('/', '_') + "#" + str(actual_revision) + file_path[-4:]
		self._p4.run( "print", "-q", "-o", stt.TMP_FOLDER_LOCATION + "/" + file_name, file_path + "#" + str(actual_revision))

		file_name = file_path[22:-4].replace ('/', '_') + "#" + str(int(actual_revision)-1) + file_path[-4:]
		self._p4.run( "print", "-q", "-o", stt.TMP_FOLDER_LOCATION + "/" + file_name, file_path + "#" + str(int(actual_revision)-1))



def dump_changelist (changelist):
	perforce_manager = PerforceManager.get_instance()

	if not os.path.exists(stt.TMP_FOLDER_LOCATION):
		os.makedirs(stt.TMP_FOLDER_LOCATION)
	else:
		files = os.listdir(stt.TMP_FOLDER_LOCATION)
		import stat
		for file in files:
			os.chmod(stt.TMP_FOLDER_LOCATION + '/' + file, stat.S_IRWXU| stat.S_IRWXG| stat.S_IRWXO)
			os.remove(stt.TMP_FOLDER_LOCATION + '/' + file)

	try:
		perforce_manager.connecto_to_server ()
		
		change = perforce_manager.get_changelist_info (changelist)
		for i, actual_revision in enumerate(change[0]['rev']):
			if int(actual_revision) > 1:
				perforce_manager.dump_file_content (change[0]['depotFile'][i], actual_revision)

		perforce_manager.disconnect_from_server()
	except P4Exception as err:
		print(err)


def check_function_difference ():
	modified_files = os.listdir(stt.TMP_FOLDER_LOCATION)

	for i in range(0, len(modified_files), 2):
		os.system("CheckFunctionDifference.bat" + " " + stt.TMP_FOLDER_LOCATION + "/" + modified_files[i] + " " + 
						stt.TMP_FOLDER_LOCATION + '/' + modified_files[i+1])


def main():
	parser = argparse.ArgumentParser()
	parser.add_argument('-c', '--changelist', help="changelist num")
	args = parser.parse_args()

	if args.changelist:
		dump_changelist (args.changelist)
		check_function_difference ()
	else:
		print('no args')


if __name__ == '__main__':
	main ()
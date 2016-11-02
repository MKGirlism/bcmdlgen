/*
	今、OBJ2CMDL・PNG2CTEXだけ開発中。
	Currently, only OBJ2CMDL and PNG2CTEX are in development.
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <time.h> // 時間。
#include <arpa/inet.h> // イメージ大きさ。

// ユーザー名とパーズ。
#ifdef linux
	#include <unistd.h>
	#include <limits.h>
	#define CurDir getcwd
#elif _WIN32
	#include <direct.h>
	#define CurDir _getcwd
#endif

using namespace std;

int matCount, polCount, vtxCount;
unsigned int width, height;

// ファイルを確認して。
inline bool file_exists (const string& name) {
	ifstream f(name.c_str());
	return f.good();
}

// .mtlファイルで「mtllib」を確認して。
bool mtllib_exists (ifstream & stream, string token) {
	string line;
	bool found;
	
	while (getline(stream, line)) {
		if (line.find(token) != string::npos) {
			found = true;
		}
	}
	
	return found;
}

// mtlのファイル名を見つけて。
string get_mtl (ifstream & stream, string token) {
	string line;
	string mtlfile;
	
	while (getline(stream, line)) {
		if (line.find(token) != string::npos) {
			mtlfile = line.substr(7,line.length());
		}
	}
	
	return mtlfile;
}

// 全部ものを数えて。
int count_stuff (ifstream & stream, string token) {
	string line;
	int count = 0;
	
	while (getline(stream, line)) {
		if (line.find(token) != string::npos) {
			count++;
		}
	}
	
	return count;
}

// CMDLファイルを書きだす。
void GenCMDL (const char *name) {
	const char *ext = ".cmdl";
	
	// 新名を準備中・・・
	char res[100];
	strcpy(res, name);
	strcat(res, ext);
	
	// 今の時間を準備中・・・
	time_t timer;
	char buffer[26];
	struct tm* tm_info;
	time(&timer);
	tm_info = localtime(&timer);
	
	strftime(buffer, 26, "%Y-%m-%dT%H:%M:%S", tm_info);
	
	// ユーザー名を受け取る。
#ifdef linux
	char username[LOGIN_NAME_MAX];
	getlogin_r(username, LOGIN_NAME_MAX);
#elif _WIN32
	TCHAR  username[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	bufCharCount = INFO_BUFFER_SIZE;
	if(!GetUserName(username, &bufCharCount)) {
		printError(TEXT("GetUserName"));
	}
#endif
	
	char cCurrentPath[FILENAME_MAX];
	
	// フォルダを見つけない時。
	if (!CurDir(cCurrentPath, sizeof(cCurrentPath))) {
		printf("Error: Can't get current directory.");
		return;
	}
	
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
	
	ofstream outFile(res);
	
	// ファイルを作らない時。
	if (outFile.fail()) {
		printf("Error: Can't create file.");
		return;
	}
	
	// ファイルジェンネレーションスタート！
	outFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
	outFile << "<NintendoWareIntermediateFile>" << endl;
	outFile << "	<GraphicsContentCtr Version=\"1.3.0\" Namespace=\"\">" << endl;
	outFile << "		<EditData>" << endl;
	outFile << "			<MetaData>" << endl;
	outFile << "				<Key>MetaData</Key>" << endl;
	outFile << "				<Create Author=\"" << username << "\" Date=\"" << buffer << "\" Source=\"\" FullPathOfSource=\"" << cCurrentPath << "\">" << endl;
	outFile << "					<ToolDescriptions Name=\"BCMDLGen\" Version=\"0.1\" />" << endl;
	outFile << "				</Create>" << endl;
	outFile << "				<Modify Date=\"" << buffer << "\">" << endl;
	outFile << "					<ToolDescriptions Name=\"BCMDLGen\" Version=\"0.1\" />" << endl;
	outFile << "				</Modify>" << endl;
	outFile << "			</Metadata>" << endl;
	outFile << "			<ContentSummaryMetaData>" << endl;
	outFile << "				<Key>ContentSummaries</Key>" << endl;
	outFile << "				<Values>" << endl;
	outFile << "					<ContentSummary ContentTypeName=\"GraphicsContent\">" << endl;
	outFile << "						<ObjectSummaries>" << endl;
	outFile << "							<ObjectSummary TypeName=\"SkelectalModel\" Name=\"\">" << endl;
	outFile << "								<Notes>" << endl;
	outFile << "									<Note Name=\"MaterialCount\" Value=\"" << matCount << "\" />" << endl;
	outFile << "									<Note Name=\"ShapeCount\" Value=\"" << matCount << "\" />" << endl;
	outFile << "									<Note Name=\"BoneCount\" Value=\"1\" />" << endl;
	outFile << "									<Note Name=\"TotalPrimitiveSetCount\" Value=\"" << matCount << "\" />" << endl;
	outFile << "									<Note Name=\"TotalNoneSkinningPrimitiveSetCount\" Value=\"" << matCount << "\" />" << endl;
	outFile << "									<Note Name=\"TotalRigidSkinningPrimitiveSetCount\" Value=\"0\" />" << endl;
	outFile << "									<Note Name=\"TotalSmoothSkinningPrimitiveSetCount\" Value=\"0\" />" << endl;
	outFile << "									<Note Name=\"TotalIndexStreamCount\" Value=\"" << matCount << "\" />" << endl;
	outFile << "									<Note Name=\"TotalPolygonCount\" Value=\"" << polCount << "\">" << endl;
	outFile << "									<Note Name=\"TotalVertexCount\" Value=\"" << vtxCount << "\">" << endl;
	outFile << "								</Notes>" << endl;
	outFile << "							</ObjectSummary>" << endl;
	outFile << "						</ObjectSummaries>" << endl;
	outFile << "					</ContentSummary>" << endl;
	outFile << "				</Values>" << endl;
	outFile << "			</ContentSummaryMetaData>" << endl;
	outFile << "		</EditData>" << endl;
	
	outFile.close();
}

void GenCTEX (const char *name, const char *source) {
	const char *ext = ".ctex";
	
	// 新名を準備中・・・
	char res[100];
	strcpy(res, name);
	strcat(res, ext);
	
	// 今の時間を準備中・・・
	time_t timer;
	char buffer[26];
	struct tm* tm_info;
	time(&timer);
	tm_info = localtime(&timer);
	
	strftime(buffer, 26, "%Y-%m-%dT%H:%M:%S", tm_info);
	
	// ユーザー名を受け取る。
#ifdef linux
	char username[LOGIN_NAME_MAX];
	getlogin_r(username, LOGIN_NAME_MAX);
#elif _WIN32
	TCHAR  username[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	bufCharCount = INFO_BUFFER_SIZE;
	if(!GetUserName(username, &bufCharCount)) {
		printError(TEXT("GetUserName"));
	}
#endif
	
	char cCurrentPath[FILENAME_MAX];
	
	// フォルダを見つけない時。
	if (!CurDir(cCurrentPath, sizeof(cCurrentPath))) {
		printf("Error: Can't get current directory.");
		return;
	}
	
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
	
	ofstream outFile(res);
	
	// ファイルを作らない時。
	if (outFile.fail()) {
		printf("Error: Can't create file.");
		return;
	}
	
	// ファイルジェンネレーションスタート！
	outFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
	outFile << "<NintendoWareIntermediateFile>" << endl;
	outFile << "	<GraphicsContentCtr Version=\"1.3.0\" Namespace=\"\">" << endl;
	outFile << "		<EditData>" << endl;
	outFile << "			<MetaData>" << endl;
	outFile << "				<Key>MetaData</Key>" << endl;
	outFile << "				<Create Author=\"" << username << "\" Date=\"" << buffer << "\" Source=\"" << source << "\" FullPathOfSource=\"" << cCurrentPath << "/" << source << "\">" << endl;
	outFile << "					<ToolDescriptions Name=\"BCMDLGen\" Version=\"0.1\" />" << endl;
	outFile << "				</Create>" << endl;
	outFile << "			</Metadata>" << endl;
	outFile << "			<ContentSummaryMetaData>" << endl;
	outFile << "				<Key>ContentSummaries</Key>" << endl;
	outFile << "				<Values>" << endl;
	outFile << "					<ContentSummary ContentTypeName=\"GraphicsContent\">" << endl;
	outFile << "						<ObjectSummary TypeName=\"ImageTexture\" Name=\"\">" << endl;
	outFile << "							<Notes>" << endl;
	outFile << "								<Note Name=\"EtcEncoding\" Value=\"Fast Perceptual\">" << endl;
	outFile << "							</Notes>" << endl;
	outFile << "						</ObjectSummary>" << endl;
	outFile << "					</ContentSummary>" << endl;
	outFile << "				</Values>" << endl;
	outFile << "			</ContentSummaryMetaData>" << endl;
	outFile << "		</EditData>" << endl;
	outFile << "		<Textures>" << endl;
	outFile << "			<ImageTextureCtr Name=\"\" Width=\"" << width << "\" Height=\"" << height << "\" MipmapSize=\"1\" Path=\"" << cCurrentPath <<"\" Encoding=\"Base64\" Format=\"Etc1\">" << endl;
	outFile << "				<Images>" << endl;
	outFile << "					<PixelBasedImageCtr>" << "</PixelBasedImageCtr>" << endl;
	outFile << "				</Images>" << endl;
	outFile << "			</ImageTextureCtr>" << endl;
	outFile << "		</Textures>" << endl;
	outFile << "	</GraphicsContentCtr>" << endl;
	outFile << "</NintendoWareIntermediateFile>";
	
	outFile.close();
}

void obj2cmdl (const char *file, const char *name) {
	const char *ext = ".mtl";
	char res[100];
	strcpy(res, name);
	strcat(res, ext);
	
	// ファイルの存在を確認して。
	if (file_exists(file)) {
		ifstream objfile, objfile2, objfile3, objfile4, mtlfile;
		objfile.open(file);
		
		// 「mtllib」を探して。
		if (!mtllib_exists (objfile, "mtllib")) {
			printf ("Can't find material lib.\n");
			return;
		}
		else {
			printf ("Material lib found, proceed.\n");
			objfile2.open(file);
			
			mtlfile.open(get_mtl (objfile2, "mtllib").c_str());
			objfile3.open(file);
			objfile4.open(file);
			
			matCount = count_stuff (mtlfile, "newmtl");
			polCount = count_stuff (objfile3, "v ");
			vtxCount = count_stuff (objfile4, "vt ");
			
			objfile.close();
			objfile2.close();
			objfile3.close();
			objfile4.close();
			mtlfile.close();
			
			// CMDLファイルを作り。
			GenCMDL (name);
		}
	}
	else {
		printf("Not found.\n");
	}
	
	//string haystack(file);
}

void dae2cmdl (const char *file) {
	printf("Coming soon.");
}

void cmdl2bin (const char *file) {
	printf("Coming soon.");
}

void tga2tex (const char *file) {
	printf("Coming soon.");
}

void png2tex (const char *file, const char *name) {
	// ファイルの存在を確認して。
	if (file_exists(file)) {
		ifstream imgfile, objfile2, objfile3, objfile4, mtlfile;
		imgfile.open(file);
		
		imgfile.seekg(16);
		imgfile.read((char *)&width, 4);
		imgfile.read((char *)&height, 4);
		
		width = ntohl(width);
		height = ntohl(height);
		
		// CMDLファイルを作り。
		GenCTEX (name, file);
	}
	else {
		printf("Not found.\n");
	}
}

void bin2obj (const char *file) {
	printf("Coming soon.");
}

void bin2dae (const char *file) {
	printf("Coming soon.");
}

void help (char *arg) {
	cout << "Usage:\n\n";
	
	cout << arg << " -h\t\t\t\t-\tDisplay help.\n";
	cout << arg << " -oc [FILE].obj [NAME]\t-\tConvert obj to cmdl (Intermediate File).\n";
	cout << arg << " -dc [FILE].dae [NAME]\t-\tConvert dae to cmdl (Intermediate File).\n";
	cout << arg << " -cb [FILE].cmdl\t\t-\tConvert cmdl to bcmdl (Binary File).\n";
	cout << arg << " -tt [FILE].tga\t\t-\tConvert tga to ctex.\n";
	cout << arg << " -pt [FILE].png\t\t-\tConvert png to ctex.\n";
	cout << arg << " -bo [FILE].bcmdl\t\t-\tConvert bcmdl to obj.\n";
	cout << arg << " -bd [FILE].bcmdl\t\t-\tConvert bcmdl to dae.\n";
}

int main (int argc, char *argv[]) {
	printf("BCMDL Generator 0.1 by MKGirlism\n\n");
	
	while (true) {
		if (argv[1] == NULL) {
			help (argv[0]);
			break;
		}
		else if (string(argv[1]) == "-h") {
			argv[1] = NULL;
			continue;
		}
		else if (string(argv[1]) == "-oc") {
			obj2cmdl (argv[2], argv[3]);
			break;
		}
		else if (string(argv[1]) == "-dc") {
			dae2cmdl (argv[2]);
			break;
		}
		else if (string(argv[1]) == "-cb") {
			cmdl2bin (argv[2]);
			break;
		}
		else if (string(argv[1]) == "-tt") {
			tga2tex (argv[2]);
			break;
		}
		else if (string(argv[1]) == "-pt") {
			png2tex (argv[2], argv[3]);
			break;
		}
		else if (string(argv[1]) == "-bo") {
			bin2obj (argv[2]);
			break;
		}
		else if (string(argv[1]) == "-bd") {
			bin2dae (argv[2]);
			break;
		}
		else {
			argv[1] = NULL;
			continue;
		}
	}
	
	printf("\nPress any key to continue...");
	cin.ignore();
	
	return 0;
}

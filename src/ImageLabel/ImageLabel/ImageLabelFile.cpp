#include "stdafx.h"
#include "ImageLabelFile.h"
#include <list>
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"
#include  <afxpriv.h>

#define RESTRICT_FRAME(value) value>1?(1):(value<0?0:value)
CImageLabelFileIOController* CImageLabelFileIOController::_instance = NULL;
CImageLabelFileIOController::Garbor CImageLabelFileIOController::garbor;
char* CreateUTF8TextInitWithString(CString strValue)
{
	char *buffer = NULL;
	int  length;

#ifdef _UNICODE  
	length = WideCharToMultiByte(CP_UTF8, 0, strValue, -1, NULL, 0, NULL, NULL);
#else  
	return NULL;
#endif  
	if (length <= 0)
	{
		return NULL;
	}
	buffer = new char[length];
	if (buffer == NULL)
	{
		return NULL;
	}
	ZeroMemory(buffer, length);
#ifdef _UNICODE  
	WideCharToMultiByte(CP_UTF8, 0, strValue, -1, buffer, length, NULL, NULL);
#else  
	strcpy_s(buffer, length, strValue);
#endif  
	return buffer;
}
BOOL CreateString_InitWithUTF8Text(CString& str, const char* pUTF8Text)
{
	if (NULL == pUTF8Text)
	{
		return FALSE;
	}
	int  unicodeLen = ::MultiByteToWideChar(CP_UTF8,
		0,
		pUTF8Text,
		-1,
		NULL,
		0);
	wchar_t*  pUnicode = new  wchar_t[unicodeLen + 1];
	if (NULL == pUnicode)
	{
		return FALSE;
	}
	MultiByteToWideChar(CP_UTF8, 0, pUTF8Text, -1, (LPWSTR)pUnicode, unicodeLen);

	str = pUnicode;

	delete[]pUnicode;
	return TRUE;
}

BOOL TinyXMLElementSetCStringAttribute(TiXmlElement* e, char* attr, CString value)
{
	char*utf_buffer;
	utf_buffer = CreateUTF8TextInitWithString(value);
	e->SetAttribute(attr, utf_buffer);
	free(utf_buffer);
	return TRUE;
}
BOOL TinyXMLElementSetCStringValue(TiXmlElement *e, CString value)
{
	char*utf_buffer;
	utf_buffer = CreateUTF8TextInitWithString(value);
	e->LinkEndChild(new TiXmlText(utf_buffer));
	free(utf_buffer);
	return TRUE;
}
FRect GetFrect(CRect drawRect, CRect winRect, CRect imgRect)
{
	FRect relativeRect;
	int validHeight;
	int validWidth;
	int validTop;
	int validBottom;
	int validLeft;
	int validRight;
	int windowHeight = winRect.Height();
	int windowWidth = winRect.Width();
	int imgHeight = imgRect.Height();
	int imgWidth = imgRect.Width();
	double standardK = imgHeight*1.0 / imgWidth;
	if (standardK < windowHeight*1.0 / windowWidth)
	{
		//如果按照宽缩放(宽固定，高多出来)
		validHeight = (int)(windowWidth*standardK);
		validWidth = windowWidth;
		validTop = (windowHeight - validHeight) / 2;
		validBottom = validTop + validHeight;
		validLeft = 0;
		validRight = windowWidth;
		relativeRect.top = (float)((drawRect.top - validTop)*1.0 / validHeight);
		relativeRect.bottom = (float)((drawRect.bottom - validTop)*1.0 / validHeight);
		relativeRect.left = (float)(drawRect.left*1.0 / validWidth);
		relativeRect.right = (float)(drawRect.right*1.0 / validWidth);
	}
	else
	{
		//如果按照高缩放(高固定，宽多出来)
		validHeight = windowHeight;
		validWidth = (int)(windowHeight / standardK);
		validTop = 0;
		validBottom = windowHeight;
		validLeft = (windowWidth - validWidth) / 2;
		validRight = validLeft + validWidth;
		relativeRect.top = (float)(drawRect.top*1.0 / validHeight);
		relativeRect.bottom = (float)(drawRect.bottom*1.0 / validHeight);
		relativeRect.left = (float)((drawRect.left - validLeft)*1.0 / validWidth);
		relativeRect.right = (float)((drawRect.right - validLeft)*1.0 / validWidth);
	}
	return relativeRect;
}
CRect GetCRect(FRect relativeRect, CRect winRect, CRect imgRect)
{
	CRect rect;
	int validHeight;
	int validWidth;
	int validTop;
	int validBottom;
	int validLeft;
	int validRight;
	int windowHeight = winRect.Height();
	int windowWidth = winRect.Width();
	int imgHeight = imgRect.Height();
	int imgWidth = imgRect.Width();
	double standardK = imgHeight*1.0 / imgWidth;
	if (standardK < windowHeight*1.0 / windowWidth)
	{
		//如果按照宽缩放(宽固定，高多出来)
		validHeight = (int)(windowWidth*standardK);
		validWidth = windowWidth;
		validTop = (windowHeight - validHeight) / 2;
		validBottom = validTop + validHeight;
		validLeft = 0;
		validRight = windowWidth;
	}
	else
	{
		//如果按照高缩放(高固定，宽多出来)
		validHeight = windowHeight;
		validWidth = (int)(windowHeight / standardK);
		validTop = 0;
		validBottom = windowHeight;
		validLeft = (windowWidth - validWidth) / 2;
		validRight = validLeft + validWidth;
	}
	rect.left = validLeft + validWidth*relativeRect.left;
	rect.right = validLeft + validWidth*relativeRect.right;
	rect.top = validTop + validHeight*relativeRect.top;
	rect.bottom = validTop + validHeight*relativeRect.bottom;
	return rect;
}


void CImageLabelFileIOController::AddImageFile(CString fileName)
{
	if (GetIfExistFile(fileName) == false)
	{
		CImageFile file;
		file.fileName = fileName;
		imageFileList.push_back(file);
	}
		
}
void CImageLabelFileIOController::DelImageFile(CString fileName)
{
	if (GetIfExistFile(fileName))
	{
		list<CImageFile>::iterator it = GetIteratorOfFile(fileName);
		imageFileList.erase(it);
	}
}
void CImageLabelFileIOController::SetFolderPath(CString folderPath)
{
	this->folderPath = folderPath;
}
void CImageLabelFileIOController::SetStorePath(CString storePath)
{
	this->storePath = storePath;
}
list<CImageFile>::iterator     CImageLabelFileIOController::GetIteratorOfFile(CString fileName)
{
	list<CImageFile>::iterator it;
	for (it = imageFileList.begin(); it != imageFileList.end(); it++)
	{
		if ((*it).fileName == fileName)
		{
			return it;
		}
	}
	return imageFileList.end();
}

void CImageLabelFileIOController::AddImageLabel(CString fileName, CRect imageSize ,int depth,CString shot,CLabel label)
{
	list<CImageFile>::iterator it = GetIteratorOfFile(fileName);
	if (it != imageFileList.end())
	{
		it->size = imageSize;
		it->depth = depth;
		it->shot = shot;
		it->labelList.push_back(label);
	}
	else
	{
		AddImageFile(fileName);
		list<CImageFile>::iterator it = GetIteratorOfFile(fileName);
		it->depth = depth;
		it->size = imageSize;
		it->shot = shot;
		it->labelList.push_back(label);
	}
}
void CImageLabelFileIOController::ModifyImageLabel(CString fileName, int index, CString shot,CLabel label)
{
	list<CImageFile>::iterator it = GetIteratorOfFile(fileName);
	it->shot = shot;
	list<CLabel>::iterator itt = (*it).labelList.begin();
	int i = 0;
	while (i != index)
	{
		itt++;
		i++;
	}
	itt->bDifficult = label.bDifficult;
	itt->bTruncated = label.bTruncated;
	itt->domain = label.domain;
	itt->type = label.type;
	itt->name = label.name;
	itt->pose = label.pose;
}
void CImageLabelFileIOController::DeleteImageLabel(CString fileName, int index)
{
	list<CImageFile>::iterator it = GetIteratorOfFile(fileName);
	list<CLabel>::iterator itt = it->labelList.begin();
	int i = 0;
	while (i != index)
	{
		itt++;
		i++;
	}
	
	it->labelList.erase(itt);
}
CString UTF8TOUTF16(const char*src)
{
	unsigned long len = ::MultiByteToWideChar(CP_UTF8, NULL, src, -1, NULL, NULL);
	if (len == 0)
		return CString(_T(""));
	wchar_t *buffer = new wchar_t[len];
	::MultiByteToWideChar(CP_UTF8, NULL, src, -1, buffer, len);

	CString dest(buffer);
	delete[] buffer;
	return dest;
}
CString TimeConvert(long long ti)
{
	CString res;
	int hour = ti / 3600000;
	int minn = ti / 60000 - hour;
	int secc = ti / 1000 - (3600 * hour + 60 * minn);
	int ms = ti % 1000;
	res.Format(_T("%02d:%02d:%02d:%03d"), hour, minn, secc, ms);
	return res;
}
long long TimeConvert(CString str)
{
	int hour, minn, secc, ms;
	swscanf_s(str, _T("%02d:%02d:%02d:%03d"), &hour, &minn, &secc, &ms);
	long long ti = hour * 3600000 + minn * 60000 + secc * 1000 + ms;
	return ti;
}
bool FolderExists(CString s)
{
	DWORD attr;
	attr = GetFileAttributes(s);
	return (attr != (DWORD)(-1)) &&
		(attr & FILE_ATTRIBUTE_DIRECTORY);
}

// 递归创建目录， 如果目录已经存在或者创建成功返回TRUE  
static bool SuperMkDir(CString P)
{
	int len = P.GetLength();
	if (len < 2) return false;
	if ('\\' == P[len - 1])
	{
		P = P.Left(len - 1);
		len = P.GetLength();
	}
	if (len <= 0) return false;
	if (len <= 3)
	{
		if (FolderExists(P))return true;
		else return false;
	}

	if (FolderExists(P))return true;
	CString Parent;
	Parent = P.Left(P.ReverseFind('\\'));

	if (Parent.GetLength() <= 0)return false;

	bool Ret = SuperMkDir(Parent);

	if (Ret)
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = 0;
		Ret = (CreateDirectory(P, &sa) == TRUE);
		return Ret;
	}
	else
		return false;
}
bool CImageLabelFileIOController::SaveFileToXML()
{
	if (!storePath.IsEmpty())
		return SaveFileToXML(storePath);
}
bool CImageLabelFileIOController::SaveFileToXML(CString path)
{
	USES_CONVERSION;
	char * xmlPath = T2A(path);
	DeleteFile(path);
	TiXmlDocument* doc = new TiXmlDocument(xmlPath);
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	doc->LinkEndChild(decl);
	TiXmlElement* imageLabel = new TiXmlElement("ImageLabel");
	doc->LinkEndChild(imageLabel);
	TiXmlElement *folder = new TiXmlElement("Folder");
	TinyXMLElementSetCStringAttribute(folder, "path", folderPath);
	imageLabel->LinkEndChild(folder);
	list<CImageFile>::iterator it;
	CString num;
	for (it = imageFileList.begin(); it != imageFileList.end(); it++)
	{
		TiXmlElement *file = new TiXmlElement("IFile");
		TinyXMLElementSetCStringAttribute(file, "name", it->fileName);
		num.Format(_T("%d"), it->size.Width());
		TinyXMLElementSetCStringAttribute(file, "width", num );
		num.Format(_T("%d"), it->size.Height());
		TinyXMLElementSetCStringAttribute(file, "height", num);
		num.Format(_T("%d"), it->depth);
		TinyXMLElementSetCStringAttribute(file, "depth", num);
		TinyXMLElementSetCStringAttribute(file, "shot", it->shot);
		list<CLabel>::iterator itt;
		int i = 0;
		for (itt = (*it).labelList.begin(); itt != (*it).labelList.end(); itt++)
		{
			TiXmlElement* image = new TiXmlElement("Label");
			TinyXMLElementSetCStringAttribute(image, "name", itt->name);
			TinyXMLElementSetCStringAttribute(image, "domain", itt->domain);
			TinyXMLElementSetCStringAttribute(image, "type", itt->type);
			num.Format(_T("%f"), itt->frect.left);
			TinyXMLElementSetCStringAttribute(image, "left", num);
			num.Format(_T("%f"), itt->frect.top);
			TinyXMLElementSetCStringAttribute(image, "top", num);
			num.Format(_T("%f"), itt->frect.right);
			TinyXMLElementSetCStringAttribute(image, "right", num);
			num.Format(_T("%f"), itt->frect.bottom);
			TinyXMLElementSetCStringAttribute(image, "bottom", num);
			image->SetAttribute("color", itt->color);
			num.Format(_T("%d"), itt->bTruncated);
			TinyXMLElementSetCStringAttribute(image, "truncated", num);
			num.Format(_T("%d"), itt->bDifficult);
			TinyXMLElementSetCStringAttribute(image, "difficult", num);
			TinyXMLElementSetCStringAttribute(image, "pose", itt->pose);
			file->LinkEndChild(image);
		}
		folder->LinkEndChild(file);
	}
	doc->SaveFile();
	delete doc;
	return true;
}
bool CImageLabelFileIOController::SaveFileToVOCXML(CString outPath)
{
	USES_CONVERSION;
	list<CImageFile>::iterator it;
	CString num;
	for (it = imageFileList.begin(); it != imageFileList.end(); it++)
	{
		CString path = outPath;
		int len = path.GetLength();
		if (path[len - 1] != '\\')
			path += '\\';
		
		path += it->fileName.Left(it->fileName.ReverseFind('\\'));
		SuperMkDir(path);
		
		if (it->fileName.ReverseFind('\\')==-1)
			path += it->fileName.Left(it->fileName.ReverseFind('.'));
		else
			path += it->fileName.Mid(it->fileName.ReverseFind('\\'), it->fileName.ReverseFind('.') - it->fileName.ReverseFind('\\'));
		path += _T(".xml");
		char *xmlPath = T2A(path);
		TiXmlDocument* doc = new TiXmlDocument(xmlPath);
		TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
		doc->LinkEndChild(decl);
		{
			TiXmlElement* annotation = new TiXmlElement("annotation");
			doc->LinkEndChild(annotation);
			{
				{
					/*CString folder = folderPath;

					if (folder[folder.GetLength() - 1] != '\\')
						folder += '\\';*/
					CString folder = it->fileName.Left(it->fileName.ReverseFind('\\'));
					
					TiXmlElement* folderEle = new TiXmlElement("folder");
					TinyXMLElementSetCStringValue(folderEle, folder);
					annotation->LinkEndChild(folderEle);

					TiXmlElement* filenameEle = new TiXmlElement("filename");
					CString fileName = it->fileName.Mid(it->fileName.ReverseFind('\\')+1);
					TinyXMLElementSetCStringValue(filenameEle, fileName);
					annotation->LinkEndChild(filenameEle);
				}
				{
					TiXmlElement* sourceEle = new TiXmlElement("source");
					{
						TiXmlElement* dataBaseEle = new TiXmlElement("database");
						TinyXMLElementSetCStringValue(dataBaseEle, _T("NIMI Object Detection Database"));
						sourceEle->LinkEndChild(dataBaseEle);
					}
					{
						TiXmlElement* annotaEle = new TiXmlElement("annotation");
						TinyXMLElementSetCStringValue(annotaEle, _T("NIMI Football Object Detection Database"));
						sourceEle->LinkEndChild(annotaEle);
					}
					{
						TiXmlElement* imageEle = new TiXmlElement("image");
						TinyXMLElementSetCStringValue(imageEle, _T(""));
						sourceEle->LinkEndChild(imageEle);
					}
					{
						TiXmlElement* flickridEle = new TiXmlElement("flickrid");
						TinyXMLElementSetCStringValue(flickridEle , _T(""));
						sourceEle->LinkEndChild(flickridEle);
					}
					annotation->LinkEndChild(sourceEle);
				}
				{
					TiXmlElement* ownerEle = new TiXmlElement("owner");
					{
						TiXmlElement* flickridEle = new TiXmlElement("flickrid");
						TinyXMLElementSetCStringValue(flickridEle, _T(""));
						ownerEle->LinkEndChild(flickridEle);
					}
					{
						TiXmlElement* nameEle = new TiXmlElement("name");
						TinyXMLElementSetCStringValue(nameEle, _T(""));
						ownerEle->LinkEndChild(nameEle);
					}
					annotation->LinkEndChild(ownerEle);
				}
				{
					TiXmlElement* sizeEle = new TiXmlElement("size");
					{
						TiXmlElement* widthEle = new TiXmlElement("width");
						num.Format(_T("%d"), it->size.Width());
						TinyXMLElementSetCStringValue(widthEle,num);
						sizeEle->LinkEndChild(widthEle);
					}
					{
						TiXmlElement* heightEle = new TiXmlElement("height");
						num.Format(_T("%d"), it->size.Height());
						TinyXMLElementSetCStringValue(heightEle, num);
						sizeEle->LinkEndChild(heightEle);
					}
					{
						TiXmlElement* depthEle = new TiXmlElement("depth");
						num.Format(_T("%d"), it->depth);
						TinyXMLElementSetCStringValue(depthEle, num);
						sizeEle->LinkEndChild(depthEle);
					}
					annotation->LinkEndChild(sizeEle);
				}
				{
					TiXmlElement* segmEle = new TiXmlElement("segmented");
					TinyXMLElementSetCStringValue(segmEle, _T("0"));
					annotation->LinkEndChild(segmEle);
				}
				list<CLabel>::iterator itt;
				for (itt = it->labelList.begin(); itt != it->labelList.end(); itt++)
				{
					TiXmlElement* objEle = new TiXmlElement("object");
					{
						TiXmlElement * nameEle = new TiXmlElement("name");
						TinyXMLElementSetCStringValue(nameEle, itt->name);
						objEle->LinkEndChild(nameEle);
					}
					{
						TiXmlElement * poseEle = new TiXmlElement("pose");
						TinyXMLElementSetCStringValue(poseEle, itt->pose);
						objEle->LinkEndChild(poseEle);
					}
					{
						TiXmlElement * trunEle = new TiXmlElement("truncated");
						TinyXMLElementSetCStringValue(trunEle, itt->bTruncated?_T("1"):_T("0"));
						objEle->LinkEndChild(trunEle);
					}
					{
						TiXmlElement * diffEle = new TiXmlElement("difficult");
						TinyXMLElementSetCStringValue(diffEle, itt->bDifficult ? _T("1") : _T("0"));
						objEle->LinkEndChild(diffEle);
					}
					{
						TiXmlElement * bndBoxEle = new TiXmlElement("bndbox");
						{
							int xmin, ymin, xmax, ymax;
							xmin = (int)it->size.Width()*itt->frect.left;
							ymin = (int)it->size.Height()*itt->frect.top;
							xmax = (int)it->size.Width()*itt->frect.right;
							ymax = (int)it->size.Height()*itt->frect.bottom;
							{
								TiXmlElement * xminEle = new TiXmlElement("xmin");
								num.Format(_T("%d"), xmin);
								TinyXMLElementSetCStringValue(xminEle, num);
								bndBoxEle->LinkEndChild(xminEle);
							}
							{
								TiXmlElement * yminEle = new TiXmlElement("ymin");
								num.Format(_T("%d"), ymin);
								TinyXMLElementSetCStringValue(yminEle, num);
								bndBoxEle->LinkEndChild(yminEle);
							}
							{
								TiXmlElement * xmaxEle = new TiXmlElement("xmax");
								num.Format(_T("%d"), xmax);
								TinyXMLElementSetCStringValue(xmaxEle, num);
								bndBoxEle->LinkEndChild(xmaxEle);
							}
							{
								TiXmlElement * ymaxEle = new TiXmlElement("ymax");
								num.Format(_T("%d"), ymax);
								TinyXMLElementSetCStringValue(ymaxEle, num);
								bndBoxEle->LinkEndChild(ymaxEle);
							}
						}
						objEle->LinkEndChild(bndBoxEle);
					}
					annotation->LinkEndChild(objEle);
				}
			}
		}
		doc->SaveFile();
		delete doc;
	}
	
	return true;
}
bool CImageLabelFileIOController::SaveFileToMINIObjectDetectionXML(CString outPath)
{
	USES_CONVERSION;
	list<CImageFile>::iterator it;
	CString num;
	for (it = imageFileList.begin(); it != imageFileList.end(); it++)
	{
		CString path = outPath;
		int len = path.GetLength();
		if (path[len - 1] != '\\')
			path += '\\';

		path += it->fileName.Left(it->fileName.ReverseFind('\\'));
		SuperMkDir(path);

		if (it->fileName.ReverseFind('\\') == -1)
			path += it->fileName.Left(it->fileName.ReverseFind('.'));
		else
			path += it->fileName.Mid(it->fileName.ReverseFind('\\'), it->fileName.ReverseFind('.') - it->fileName.ReverseFind('\\'));
		path += _T(".xml");
		char *xmlPath = T2A(path);
		TiXmlDocument* doc = new TiXmlDocument(xmlPath);
		TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
		doc->LinkEndChild(decl);
		{
			TiXmlElement* annotation = new TiXmlElement("annotation");
			doc->LinkEndChild(annotation);
			{
				{
					/*CString folder = folderPath;
					if (folder[folder.GetLength() - 1] != '\\')
						folder += '\\';*/
					CString folder = it->fileName.Left(it->fileName.ReverseFind('\\'));

					TiXmlElement* folderEle = new TiXmlElement("folder");
					TinyXMLElementSetCStringValue(folderEle, folder);
					annotation->LinkEndChild(folderEle);

					TiXmlElement* filenameEle = new TiXmlElement("filename");
					CString fileName = it->fileName.Mid(it->fileName.ReverseFind('\\') + 1);
					TinyXMLElementSetCStringValue(filenameEle, fileName);
					annotation->LinkEndChild(filenameEle);
				}
				{
					TiXmlElement* sourceEle = new TiXmlElement("source");
					{
						TiXmlElement* dataBaseEle = new TiXmlElement("database");
						TinyXMLElementSetCStringValue(dataBaseEle, _T("NIMI Object Detection Database"));
						sourceEle->LinkEndChild(dataBaseEle);
					}
					{
						TiXmlElement* annotaEle = new TiXmlElement("annotation");
						TinyXMLElementSetCStringValue(annotaEle, _T("NIMI Football Object Detection Database"));
						sourceEle->LinkEndChild(annotaEle);
					}
					{
						TiXmlElement* imageEle = new TiXmlElement("image");
						TinyXMLElementSetCStringValue(imageEle, _T(""));
						sourceEle->LinkEndChild(imageEle);
					}
					{
						TiXmlElement* flickridEle = new TiXmlElement("flickrid");
						TinyXMLElementSetCStringValue(flickridEle, _T(""));
						sourceEle->LinkEndChild(flickridEle);
					}
					annotation->LinkEndChild(sourceEle);
				}
				{
					TiXmlElement* ownerEle = new TiXmlElement("owner");
					{
						TiXmlElement* flickridEle = new TiXmlElement("flickrid");
						TinyXMLElementSetCStringValue(flickridEle, _T(""));
						ownerEle->LinkEndChild(flickridEle);
					}
					{
						TiXmlElement* nameEle = new TiXmlElement("name");
						TinyXMLElementSetCStringValue(nameEle, _T(""));
						ownerEle->LinkEndChild(nameEle);
					}
					annotation->LinkEndChild(ownerEle);
				}
				{
					TiXmlElement* sizeEle = new TiXmlElement("size");
					{
						TiXmlElement* widthEle = new TiXmlElement("width");
						num.Format(_T("%d"), it->size.Width());
						TinyXMLElementSetCStringValue(widthEle, num);
						sizeEle->LinkEndChild(widthEle);
					}
					{
						TiXmlElement* heightEle = new TiXmlElement("height");
						num.Format(_T("%d"), it->size.Height());
						TinyXMLElementSetCStringValue(heightEle, num);
						sizeEle->LinkEndChild(heightEle);
					}
					{
						TiXmlElement* depthEle = new TiXmlElement("depth");
						num.Format(_T("%d"), it->depth);
						TinyXMLElementSetCStringValue(depthEle, num);
						sizeEle->LinkEndChild(depthEle);
					}
					annotation->LinkEndChild(sizeEle);
				}
				{
					TiXmlElement* shotEle = new TiXmlElement("shot");
					TinyXMLElementSetCStringValue(shotEle, it->shot);
					annotation->LinkEndChild(shotEle);
				}
				{
					TiXmlElement* segmEle = new TiXmlElement("segmented");
					TinyXMLElementSetCStringValue(segmEle, _T("0"));
					annotation->LinkEndChild(segmEle);
				}
				list<CLabel>::iterator itt;
				for (itt = it->labelList.begin(); itt != it->labelList.end(); itt++)
				{
					TiXmlElement* objEle = new TiXmlElement("object");
					{
						TiXmlElement * nameEle = new TiXmlElement("name");
						TinyXMLElementSetCStringValue(nameEle, itt->name);
						objEle->LinkEndChild(nameEle);
					}
					{
						TiXmlElement * poseEle = new TiXmlElement("pose");
						TinyXMLElementSetCStringValue(poseEle, itt->pose);
						objEle->LinkEndChild(poseEle);
					}
					{
						TiXmlElement * trunEle = new TiXmlElement("truncated");
						TinyXMLElementSetCStringValue(trunEle, itt->bTruncated ? _T("1") : _T("0"));
						objEle->LinkEndChild(trunEle);
					}
					{
						TiXmlElement * diffEle = new TiXmlElement("difficult");
						TinyXMLElementSetCStringValue(diffEle, itt->bDifficult ? _T("1") : _T("0"));
						objEle->LinkEndChild(diffEle);
					}
					{
						TiXmlElement * bndBoxEle = new TiXmlElement("bndbox");
						{
							int xmin, ymin, xmax, ymax;
							xmin = (int)it->size.Width()*itt->frect.left;
							ymin = (int)it->size.Height()*itt->frect.top;
							xmax = (int)it->size.Width()*itt->frect.right;
							ymax = (int)it->size.Height()*itt->frect.bottom;
							{
								TiXmlElement * xminEle = new TiXmlElement("xmin");
								num.Format(_T("%d"), xmin);
								TinyXMLElementSetCStringValue(xminEle, num);
								bndBoxEle->LinkEndChild(xminEle);
							}
							{
								TiXmlElement * yminEle = new TiXmlElement("ymin");
								num.Format(_T("%d"), ymin);
								TinyXMLElementSetCStringValue(yminEle, num);
								bndBoxEle->LinkEndChild(yminEle);
							}
							{
								TiXmlElement * xmaxEle = new TiXmlElement("xmax");
								num.Format(_T("%d"), xmax);
								TinyXMLElementSetCStringValue(xmaxEle, num);
								bndBoxEle->LinkEndChild(xmaxEle);
							}
							{
								TiXmlElement * ymaxEle = new TiXmlElement("ymax");
								num.Format(_T("%d"), ymax);
								TinyXMLElementSetCStringValue(ymaxEle, num);
								bndBoxEle->LinkEndChild(ymaxEle);
							}
						}
						objEle->LinkEndChild(bndBoxEle);
					}
					annotation->LinkEndChild(objEle);
				}
			}
		}
		doc->SaveFile();
		delete doc;
	}

	return true;
}
bool CImageLabelFileIOController::ReadFileFromXML()
{
	return ReadFileFromXML(storePath);
}
bool CImageLabelFileIOController::ReadFileFromXML(CString path)
{
	//初始化环境
	imageFileList.clear();

	USES_CONVERSION;
	char * xmlPath = T2A(path);
	storePath = path;
	TiXmlDocument* doc = new TiXmlDocument(xmlPath);
	bool bSuccess = doc->LoadFile(TIXML_ENCODING_UTF8);
	if (!bSuccess) {
		TRACE("Could not load test file %s. Error='%s'. Exiting.\n", xmlPath, doc->ErrorDesc());
		delete doc;
		return false;
	}
	TiXmlElement* root = doc->RootElement();
	TiXmlElement* folder = root->FirstChildElement("Folder");
	CString str;
	CreateString_InitWithUTF8Text(str, folder->Attribute("path"));
	folderPath = str;
	TiXmlElement* file = folder->FirstChildElement("IFile");
	int fileNum = 0;
	
	/************************************************************************/
	/*                       获得File 数量                                  */
	/************************************************************************/
	while (file != NULL)
	{
		CImageFile imageFile;
		CreateString_InitWithUTF8Text(str, file->Attribute("name"));
		imageFile.fileName = str;
		TRACE(str);
		CreateString_InitWithUTF8Text(str, file->Attribute("width"));
		int width = atoi(T2A(str));
		CreateString_InitWithUTF8Text(str, file->Attribute("height"));
		int height = atoi(T2A(str));
		CreateString_InitWithUTF8Text(str, file->Attribute("depth"));
		imageFile.depth = atoi(T2A(str));
		imageFile.size.SetRect(0, 0, width, height);
		CreateString_InitWithUTF8Text(str, file->Attribute("shot"));
		imageFile.shot = str;
		TiXmlElement* label = file->FirstChildElement("Label");
		while (label != NULL)
		{
			CLabel clabel;
			float num;
			CreateString_InitWithUTF8Text(str, label->Attribute("left"));
			num = atof(T2A(str));
			clabel.frect.left = RESTRICT_FRAME(num);
			CreateString_InitWithUTF8Text(str, label->Attribute("right"));
			num = atof(T2A(str));
			clabel.frect.right = RESTRICT_FRAME(num);
			CreateString_InitWithUTF8Text(str, label->Attribute("top"));
			num = atof(T2A(str));
			clabel.frect.top = RESTRICT_FRAME(num);
			CreateString_InitWithUTF8Text(str, label->Attribute("bottom"));
			num = atof(T2A(str));
			clabel.frect.bottom = RESTRICT_FRAME(num);
			CreateString_InitWithUTF8Text(str, label->Attribute("domain"));
			clabel.domain = str;
			CreateString_InitWithUTF8Text(str, label->Attribute("type"));
			clabel.type = str;
			CreateString_InitWithUTF8Text(str, label->Attribute("name"));
			clabel.name = str;
			CreateString_InitWithUTF8Text(str,label->Attribute("truncated"));
			clabel.bTruncated = atoi(T2A(str));
			CreateString_InitWithUTF8Text(str, label->Attribute("difficult"));
			clabel.bDifficult = atoi(T2A(str));
			CreateString_InitWithUTF8Text(str, label->Attribute("pose"));
			clabel.pose = str;
			unsigned int tmpcolor;
			label->QueryUnsignedAttribute("color", &tmpcolor);
			clabel.color = tmpcolor;
			imageFile.labelList.push_back(clabel);
			label = label->NextSiblingElement("Label");
		}
		fileNum++;
		imageFileList.push_back(imageFile);
		file = file->NextSiblingElement("IFile");
	}
	delete doc;
	return true;
}
bool CImageLabelFileIOController::GetIfExistFile(CString fileName)
{
	list<CImageFile>::iterator it;
	for (it = imageFileList.begin(); it != imageFileList.end(); it++)
	{
		if ((*it).fileName == fileName)
			return true;
	}
	return false;
}

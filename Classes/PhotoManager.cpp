#include "PhotoManager.h"
#include "cocos2d.h"
#include "../external/tinyxml2/tinyxml2.h"  

USING_NS_CC;
using namespace tinyxml2;

const int FRAGMENT_TOTAL_COUNT = 12;
const char* FRAGMENT_FILENAME = "PhotoFragmentRecord.xml";

PhotoManager* PhotoManager::_instance = nullptr;

// ˽�еĹ��ߺ���
static unsigned int _binaryStringToInt(const char* str)
{
	unsigned int result = 0;
	while(*str)
	{
		result = result * 2 + *str - '0';
		str++;
	}

	return result;
}

// TODO �����е������Ƚ������ð�
static std::string _intToBinaryString(unsigned int value)
{
	std::string result = "";

	const int totalFragment = 12;

	for(int i = 0 ; i < totalFragment ; ++i)
	{
		std::string append;

		if (value & 1 << i)
		{
			append = "1";
		}
		else
		{
			append = "0";
		}

		result = append + result;
	}

	return result;
}

PhotoManager* PhotoManager::getInstance()
{
    if (!_instance)
    {
        _instance = new PhotoManager();
        _instance->init();
    }

    return _instance;
}

void PhotoManager::init()
{
	// TODO ����д���ɣ��Ժ���˵
	PhotoFileNameList girls;
	girls.push_back("photo/girls/001.jpg");
	girls.push_back("photo/girls/002.jpg");
	girls.push_back("photo/girls/003.jpg");
	girls.push_back("photo/girls/004.jpg");
	girls.push_back("photo/girls/005.jpg");
	girls.push_back("photo/girls/006.jpg");
	girls.push_back("photo/girls/007.jpg");
	girls.push_back("photo/girls/008.jpg");
	girls.push_back("photo/girls/009.jpg");
	girls.push_back("photo/girls/010.jpg");
	girls.push_back("photo/girls/011.jpg");
	girls.push_back("photo/girls/012.jpg");
	girls.push_back("photo/girls/013.jpg");
	girls.push_back("photo/girls/014.jpg");
	girls.push_back("photo/girls/015.jpg");
	girls.push_back("photo/girls/016.jpg");
	girls.push_back("photo/girls/017.jpg");
	girls.push_back("photo/girls/018.jpg");
	girls.push_back("photo/girls/019.jpg");
	_photos.push_back(girls);

	PhotoFileNameList boys;
	boys.push_back("photo/boys/001.jpg");
	boys.push_back("photo/boys/002.jpg");
	boys.push_back("photo/boys/003.jpg");
	boys.push_back("photo/boys/004.jpg");
	boys.push_back("photo/boys/005.jpg");
	boys.push_back("photo/boys/006.jpg");
	boys.push_back("photo/boys/007.jpg");
	boys.push_back("photo/boys/008.jpg");
	boys.push_back("photo/boys/009.jpg");
	boys.push_back("photo/boys/010.jpg");
	boys.push_back("photo/boys/011.jpg");
	boys.push_back("photo/boys/012.jpg");
	boys.push_back("photo/boys/013.jpg");
	boys.push_back("photo/boys/014.jpg");
	boys.push_back("photo/boys/015.jpg");
	boys.push_back("photo/boys/016.jpg");
	boys.push_back("photo/boys/017.jpg");
	boys.push_back("photo/boys/018.jpg");
	_photos.push_back(boys);

	PhotoFileNameList animals;
	animals.push_back("photo/animals/001.jpg");
	animals.push_back("photo/animals/002.jpg");
	animals.push_back("photo/animals/003.jpg");
	animals.push_back("photo/animals/004.jpg");
	animals.push_back("photo/animals/005.jpg");
	animals.push_back("photo/animals/006.jpg");
	animals.push_back("photo/animals/007.jpg");
	animals.push_back("photo/animals/008.jpg");
	animals.push_back("photo/animals/009.jpg");
	animals.push_back("photo/animals/010.jpg");
	_photos.push_back(animals);

	PhotoFileNameList landscapes;
	landscapes.push_back("photo/landscapes/001.jpg");
	landscapes.push_back("photo/landscapes/002.jpg");
	landscapes.push_back("photo/landscapes/003.jpg");
	landscapes.push_back("photo/landscapes/004.jpg");
	landscapes.push_back("photo/landscapes/005.jpg");
	landscapes.push_back("photo/landscapes/006.jpg");
	landscapes.push_back("photo/landscapes/007.jpg");
	_photos.push_back(landscapes);

	photoFragmentReadFromFile();
}

unsigned int PhotoManager::getFragmentMask(const std::string& photoFileName)
{
	for(auto& photoFragmentProperty : _photoFragments)
	{
		if (photoFragmentProperty.photoFileName == photoFileName)
		{
			return photoFragmentProperty.FragmentMask;
		}
	}

	return 0;
}

void PhotoManager::addFragmentMask(const std::string& photoFileName,unsigned int newFragmentMask)
{
	for(auto& photoFragmentProperty : _photoFragments)
	{
		if (photoFragmentProperty.photoFileName == photoFileName)
		{
			photoFragmentProperty.FragmentMask |= newFragmentMask;

			// ͬ�����ļ�
			photoFragmentFlushToFile(photoFragmentProperty);
			return;
		}
	}

	// ������������˵�����ǻ�õĵ�һ����Ƭ
	PhotoFragmentRecord record;
	record.photoFileName = photoFileName;
	record.FragmentMask = newFragmentMask;
	_photoFragments.push_back(record);

	// ͬ�����ļ�
	photoFragmentFlushToFile(record);
}

void PhotoManager::delFragment(const std::string& photoFileName)
{
	bool found = false;
	for(auto iter = _photoFragments.begin(); iter != _photoFragments.end() ; ++iter)
	{
		if (iter->photoFileName == photoFileName)
		{
			_photoFragments.erase(iter);
			found = true;
			break;
		}
	}

	// ȥ�ļ���Ҳɾ��
	if (found)
	{
		std::string fileFullName = CCFileUtils::getInstance()->getWritablePath() + FRAGMENT_FILENAME;

		if (!CCFileUtils::getInstance()->isFileExist(fileFullName))
		{
			return;
		}
		else
		{
			tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
			if (pDoc == nullptr)
			{
				return;
			}

			pDoc->LoadFile(fileFullName.c_str());

			//�õ����ڵ�
			XMLElement *rootElement = pDoc->RootElement();

			// ��ȡ��һ���ӽڵ�
			XMLElement *photoElement = rootElement->FirstChildElement();
			while(photoElement)
			{
				if (photoFileName == photoElement->Attribute("name"))
				{
					photoElement->Parent()->DeleteChild(photoElement);
					break;
				}

				// ��һ���ڵ�
				photoElement = photoElement->NextSiblingElement();
			}

			pDoc->SaveFile(fileFullName.c_str());

			delete pDoc;
		}
	}
}

void PhotoManager::photoFragmentReadFromFile()
{
	_photoFragments.clear();

	std::string fileFullName = CCFileUtils::getInstance()->getWritablePath() + FRAGMENT_FILENAME;

	if (!CCFileUtils::getInstance()->isFileExist(fileFullName))
	{
		return;
	}
	else
	{
		tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
		if (pDoc == nullptr)
		{
			return;
		}

		pDoc->LoadFile(fileFullName.c_str());

		//�õ����ڵ�
		XMLElement *rootElement = pDoc->RootElement();

		// ��ȡ��һ���ӽڵ�
		XMLElement *photoElement = rootElement->FirstChildElement();
		while(photoElement)
		{
			PhotoFragmentRecord record;
			record.photoFileName = photoElement->Attribute("name");
			const char* fragmentString = photoElement->Attribute("fragment");
			record.FragmentMask = _binaryStringToInt(fragmentString);
			_photoFragments.push_back(record);

			// ��һ���ڵ�
			photoElement = photoElement->NextSiblingElement();
		}

		delete pDoc;
	}
}

void PhotoManager::photoFragmentFlushToFile(const PhotoFragmentRecord& photoFragments)
{
	std::string fileFullName = CCFileUtils::getInstance()->getWritablePath() + FRAGMENT_FILENAME;

	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	if (pDoc == nullptr)
	{
		return;
	}

	// ����ļ������ڣ���ô�½�һ���ļ������ѵ�ǰ��¼д��ȥ
	if (!CCFileUtils::getInstance()->isFileExist(fileFullName))
	{
		// xml����
		XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
		if ( pDel == nullptr ) 
		{
			return ;
		}

		pDoc->LinkEndChild(pDel);

		// �ڵ�fragments
	    XMLElement *fragmentsElement = pDoc->NewElement("fragments");
		pDoc->LinkEndChild(fragmentsElement);

		// �ڵ�photo
		XMLElement *photoElement = pDoc->NewElement("photo");
		photoElement->SetAttribute("name", photoFragments.photoFileName.c_str());
		std::string fragmentMaskString = _intToBinaryString(photoFragments.FragmentMask);
		photoElement->SetAttribute("fragment", fragmentMaskString.c_str());
		fragmentsElement->LinkEndChild(photoElement);

		pDoc->SaveFile(fileFullName.c_str());
	}
	else
	{
		pDoc->LoadFile(fileFullName.c_str());

		//�õ����ڵ�
		XMLElement *rootElement = pDoc->RootElement();

		// ��ȡ��һ���ӽڵ�
		XMLElement *photoElement = rootElement->FirstChildElement();

		bool isFound = false;
		while(photoElement)
		{
			if (photoFragments.photoFileName == photoElement->Attribute("name"))
			{
				isFound = true;
				std::string fragmentMaskString = _intToBinaryString(photoFragments.FragmentMask);
				photoElement->SetAttribute("fragment", fragmentMaskString.c_str());
				break;
			}

			// ��һ���ڵ�
			photoElement = photoElement->NextSiblingElement();
		}

		if (!isFound)
		{
			// û�ҵ��ʹ���һ���½ڵ�
			XMLElement *photoElement = pDoc->NewElement("photo");
			photoElement->SetAttribute("name", photoFragments.photoFileName.c_str());
			std::string fragmentMaskString = _intToBinaryString(photoFragments.FragmentMask);
			photoElement->SetAttribute("fragment", fragmentMaskString.c_str());
			rootElement->LinkEndChild(photoElement);
		}

		pDoc->SaveFile(fileFullName.c_str());
	}

	delete pDoc;
}



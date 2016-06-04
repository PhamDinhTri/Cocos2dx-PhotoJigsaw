#ifndef __FRAME_PHOTOMANAGER_H__
#define __FRAME_PHOTOMANAGER_H__

#include <vector>
#include <string>

class PhotoManager
{
	typedef std::vector<std::string> PhotoFileNameList;

	typedef struct tagPhotoFragmentRecord
	{
		std::string photoFileName;
		unsigned int FragmentMask;		// ÿλ����ʶ��Ƭ�Ƿ�õ���0:û�õ���1:�ѵõ���
	}PhotoFragmentRecord;
	typedef std::vector<PhotoFragmentRecord> PhotoFragmentRecordList;
public:
	static PhotoManager* getInstance();

	// ��Ƭ��غ���
	std::vector<PhotoFileNameList>& getAllPhotos(){return _photos;}

	// ��Ƭ��غ���
	PhotoFragmentRecordList& getAllPhotoFragments(){return _photoFragments;}
	unsigned int getFragmentMask(const std::string& photoFileName);
	void addFragmentMask(const std::string& photoFileName,unsigned int newFragmentMask);
	void delFragment(const std::string& photoFileName);
private:
	void init();

	void photoFragmentReadFromFile();
	void photoFragmentFlushToFile(const PhotoFragmentRecord& photoFragments);

	static PhotoManager* _instance;

	std::vector<PhotoFileNameList> _photos;
	PhotoFragmentRecordList _photoFragments;
};

#endif // __FRAME_PHOTOMANAGER_H__
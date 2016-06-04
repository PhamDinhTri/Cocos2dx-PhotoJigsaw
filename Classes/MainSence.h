#ifndef __FRAME_MAINSCENE_H__
#define __FRAME_MAINSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

struct PointCmp
{
	bool operator()(const cocos2d::Point& _Left, const cocos2d::Point& _Right)
	{
		if (_Left.x == _Right.x)
		{
			return _Left.y < _Right.y;
		}
		else
		{
			return _Left.x < _Right.x;
		}
	}
};

class MainSence : public cocos2d::Layer
{
public:
    static cocos2d::Scene* scene(const std::string& jigsawFileName);
	CREATE_FUNC(MainSence);

	virtual bool init() override;
	virtual void onExit() override;

	void backButtonTouched(Ref *pSender, cocos2d::ui::TouchEventType type);
	void jigsawLayerTouched(Ref *pSender, cocos2d::ui::TouchEventType type);
	void testButtonTouched(Ref *pSender, cocos2d::ui::TouchEventType type);
	void itemButtonTouched(Ref *pSender, cocos2d::ui::TouchEventType type);
	void originalButtonTouched(Ref *pSender, cocos2d::ui::TouchEventType type);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
private:
	MainSence();
	// ��ʼ��ƴͼ����������˳��
	void initJigsaw(const std::string& jigsawFileName);

	// ��������λ�û�ȡ��Ƭ
	cocos2d::Sprite* getSpriteByIndexPoint(cocos2d::Point index);

	// ����2������λ�õ���Ƭ,swap�����л����swapBegin��swapEnd
	void swap(cocos2d::Point from,cocos2d::Point fromOriginalPosition,cocos2d::Point to,bool instant = true);
	void swapBegin();
	void swapEnd();

	// ����1��2ͼ��Ϊ������
	void setSwapAnyInvalid();

	// �������ƴͼ��͸���ȣ����λ����ȷ����ô���ø�͸���ȣ��������õ�͸����
	void setAllJigsawOpacity();

	// �ж��Ƿ�ʤ��
	void checkVictory();

	// ���ص��ϼ�����
	void backToUpperUI();

	// ƴͼ�������
	std::string _jigsawFileName;			// ƴͼ���ļ���
	cocos2d::ui::Layout* _jigsawLayout;		// ���ƴͼ��layout
	int _photoWidth;						// ��Ƭ���
	int _photoHeight;						// ��Ƭ�߶�
	int _piecesInWidth;						// ������Ƭ����
	int _piecesInHeight;					// ������Ƭ����
	float _jigsawWidth;						// ƴͼ���
	float _jigsawHeight;					// ƴͼ�߶�
	int _gapWidth;							// ��϶���

	//				 ��Ƭ��ǰ������λ��		  ��ƬSprite	   ��Ƭ��ȷ������λ��
	typedef std::map<cocos2d::Point,std::pair<cocos2d::Sprite*,cocos2d::Point>,PointCmp> JigsawMap;
	JigsawMap _jigsawMap;

	// �ƶ��뽻����ص����Ա����
	bool _isValidSwap;		// ֻ���ڿհ׿���Χ��ƴͼ�ƶ�ʱ��������Ч���ƶ�
	bool _isSwapLock;		// �����Ƿ������ˣ��ڽ�����������ʱ������ס
	int _swapEndCount;
	enum class BlankPosition
    {
		ON_YOUR_RIGHT = 0,			// �հ׿��ڱ���ѡ����Ƭ���ұ�
        ON_YOUR_LEFT,				// �հ׿��ڱ���ѡ����Ƭ�����
		ON_YOUR_UP,					// �հ׿��ڱ���ѡ����Ƭ���ϱ�
		ON_YOUR_DOWN,				// �հ׿��ڱ���ѡ����Ƭ���±�
    };
	BlankPosition _blankPosition;
	cocos2d::Sprite* _fromSprite;			// from��Sprite��ֻ��Ϊ��Moveʱ����ֱ�������ã������ٲ��ˣ�
	cocos2d::Point _fromOriginalPosition;	// from��ԭʼλ�ã�Moveʱ��λ�û�䣬��˱��������
	cocos2d::Point _swapFromJigsaw;	// ��������ʱfrom������λ��
	cocos2d::Point _swapToJigsaw;	// ��������ʱto������λ��

	// ���⽻��ģʽ
	cocos2d::ui::Button* _itemButton;		// ���߰�ť
	cocos2d::Sprite* _swap1Sprite;
	cocos2d::Sprite* _swap2Sprite;
	bool _inSwapAnyMode;
	bool _swap1Touched;			// ���⽻��ģʽ�£���һ���Ƿ��Ѿ�����

	// ԭͼ����
	cocos2d::ui::Layout* _containerLayout;	// ƴͼ��ԭͼ��������
	cocos2d::ui::Layout* _originalLayout;	// ���ԭͼ��layout
	cocos2d::ui::Button* _originalButton;		// ���߰�ť

	// �ɼ����
	int _moveStep;
	unsigned long _startTime;
	unsigned long _winTime;
	int _useItem;
};

#endif // __FRAME_MAINSCENE_H__
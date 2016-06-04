#include "SetupLayer.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

bool SetupLayer::init()
{
	if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 150)))
    {
        return false;
    }

	// ��json�е���ui
	Layout* mainPanel = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(
		FileUtils::getInstance()->fullPathForFilename("ui/SetupLayout.json").c_str()));

	// ��ť�󶨻ص�
	auto quitButton = static_cast<Button*>(mainPanel->getChildByName("QuitButton"));
	quitButton->addTouchEventListener(this, toucheventselector(SetupLayer::quitButtonTouched));

	// �������ְ�ť״̬
	auto musicButton = static_cast<Button*>(mainPanel->getChildByName("MusicButton"));
	musicButton->addTouchEventListener(this, toucheventselector(SetupLayer::musicButtonTouched));

	bool lastTimeMusic = UserDefault::getInstance()->getBoolForKey("SetupMusic",true);
	if (lastTimeMusic)
	{
		musicButton->setBright(true);
	}
	else
	{
		musicButton->setBright(false);
	}

	// ������Ч��ť״̬
	auto effectButton = static_cast<Button*>(mainPanel->getChildByName("EffectButton"));
	effectButton->addTouchEventListener(this, toucheventselector(SetupLayer::effectButtonTouched));

	bool lastTimeEffect = UserDefault::getInstance()->getBoolForKey("SetupEffect",true);
	if (lastTimeEffect)
	{
		effectButton->setBright(true);
	}
	else
	{
		effectButton->setBright(false);
	}

	// ��������
	auto newbieGuideCheckBox = static_cast<CheckBox*>(mainPanel->getChildByName("NewbieGuideCheckBox"));
	newbieGuideCheckBox->addEventListenerCheckBox(this,checkboxselectedeventselector(SetupLayer::newbieGuideCheckBoxTouched));

	bool newbieGuide = UserDefault::getInstance()->getBoolForKey("NewbieGuide",true);
	if (newbieGuide)
	{
		newbieGuideCheckBox->setSelectedState(true);
	}
	else
	{
		newbieGuideCheckBox->setSelectedState(false);
	}

	this->addChild(mainPanel);

	// ģ̬Ч��
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	_listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = CC_CALLBACK_2(SetupLayer::onTouchBegan,this);
	_listener->setSwallowTouches(true);
	dispatcher->addEventListenerWithSceneGraphPriority(_listener,this);

	return true;
}

bool SetupLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void SetupLayer::quitButtonTouched(Ref *pSender, TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED)
	{
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		dispatcher->removeEventListener(_listener);

		this->removeFromParentAndCleanup(false);
	}
}

void SetupLayer::musicButtonTouched(Ref *pSender, cocos2d::ui::TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED)
	{
		auto musicButton = dynamic_cast<Button*>(pSender);
		if (musicButton->isBright())
		{
			musicButton->setBright(false);
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			UserDefault::getInstance()->setBoolForKey("SetupMusic", false);
		}
		else
		{
			musicButton->setBright(true);
			if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
			{
				SimpleAudioEngine::getInstance()->playBackgroundMusic(FileUtils::getInstance()->fullPathForFilename(MUSIC_FILE).c_str(), true);
			}
			else
			{
				SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			}
			UserDefault::getInstance()->setBoolForKey("SetupMusic", true);
		}
	}
}
void SetupLayer::effectButtonTouched(Ref *pSender, cocos2d::ui::TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED)
	{
		auto effectButton = dynamic_cast<Button*>(pSender);
		if (effectButton->isBright())
		{
			effectButton->setBright(false);
			UserDefault::getInstance()->setBoolForKey("SetupEffect", false);
		}
		else
		{
			effectButton->setBright(true);
			UserDefault::getInstance()->setBoolForKey("SetupEffect", true);
		}
	}
}

void SetupLayer::newbieGuideCheckBoxTouched(Ref *pSender, cocos2d::ui::CheckBoxEventType type)
{
	if (type == CHECKBOX_STATE_EVENT_SELECTED)
	{
		UserDefault::getInstance()->setBoolForKey("NewbieGuide", true);
	}
	else if (type == CHECKBOX_STATE_EVENT_UNSELECTED)
	{
		UserDefault::getInstance()->setBoolForKey("NewbieGuide", false);
	}
	else
	{
	}
}

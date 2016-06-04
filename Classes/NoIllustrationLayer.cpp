#include "NoIllustrationLayer.h"
#include "GameStartSence.h"

USING_NS_CC;
using namespace ui;

bool NoIllustrationLayer::init()
{
	if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 150)))
    {
        return false;
    }

	// ��json�е���ui
	Layout* mainPanel = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(
		FileUtils::getInstance()->fullPathForFilename("ui/NoIllustrationLayout.json").c_str()));

	// ��ť�󶨻ص�
	auto okButton = static_cast<Button*>(mainPanel->getChildByName("OKButton"));
	okButton->addTouchEventListener(this, toucheventselector(NoIllustrationLayer::okButtonTouched));

	this->addChild(mainPanel);

	// ģ̬Ч��
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	_listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = CC_CALLBACK_2(NoIllustrationLayer::onTouchBegan,this);
	_listener->setSwallowTouches(true);
	dispatcher->addEventListenerWithSceneGraphPriority(_listener,this);

	return true;
}

bool NoIllustrationLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void NoIllustrationLayer::okButtonTouched(Ref *pSender, TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED)
	{
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		dispatcher->removeEventListener(_listener);

		auto newSence = TransitionFade::create(1.5f,GameStartSence::scene());
		if (newSence)
		{
			Director::getInstance()->replaceScene(newSence);
		}
	}
}

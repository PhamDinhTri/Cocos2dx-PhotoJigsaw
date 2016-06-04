#include "VictorySence.h"
#include "PhotoSelectSence.h"
#include "IllustrationSence.h"
#include "PhotoManager.h"

USING_NS_CC;
using namespace ui;

Scene* VictorySence::scene(const std::string& jigsawFileName,int useTime,int useStep,int useItem)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    VictorySence *layer = VictorySence::create();
	layer->setScore(jigsawFileName,useTime,useStep,useItem);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool VictorySence::init()
{
	if ( !Layer::init() )
    {
        return false;
    }

	// ��json�е���ui
	Layout* mainPanel = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(
		FileUtils::getInstance()->fullPathForFilename("ui/VictoryLayout.json").c_str()));

	_evaluationClassPanel = static_cast<Layout*>(mainPanel->getChildByName("EvaluationClassPanel"));
	_fragmentDisplayPanel = static_cast<Layout*>(mainPanel->getChildByName("FragmentDisplayPanel"));
	_useTimeText = static_cast<TextBMFont*>(mainPanel->getChildByName("UseTimeLabel"));
	_useStepText = static_cast<TextBMFont*>(mainPanel->getChildByName("UseStepLabel"));
	_useItemText = static_cast<TextBMFont*>(mainPanel->getChildByName("UseItemLabel"));
	_getFragmentImage = static_cast<ImageView*>(mainPanel->getChildByName("GetFragmentImage"));
	_noGetFragmentImage = static_cast<ImageView*>(mainPanel->getChildByName("NoGetFragmentImage"));

	// ��ť�󶨻ص�
	_playAgainButton = static_cast<Button*>(mainPanel->getChildByName("PlayAgainButton"));
	_playAgainButton->addTouchEventListener(this, toucheventselector(VictorySence::playAgainButtonTouched));

	_illustrationButton = static_cast<Button*>(mainPanel->getChildByName("IllustrationButton"));
	_illustrationButton->addTouchEventListener(this, toucheventselector(VictorySence::illustrationButtonTouched));

	// ��ť����Ƭ��ȫ������Ϊ������
	_getFragmentImage->setVisible(false);
	_noGetFragmentImage->setVisible(false);
	_fragmentDisplayPanel->setVisible(false);
	_playAgainButton->setVisible(false);
	_illustrationButton->setVisible(false);
	_playAgainButton->setTouchEnabled(false);
	_illustrationButton->setTouchEnabled(false);

	this->addChild(mainPanel,0,0);

	return true;
}

void VictorySence::playAgainButtonTouched(Ref *pSender, cocos2d::ui::TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED)
	{
		auto newSence = TransitionFade::create(1.f,PhotoSelectSence::scene());
		if (newSence)
		{
			Director::getInstance()->replaceScene(newSence);
		}
	}
}

void VictorySence::illustrationButtonTouched(Ref *pSender, TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED)
	{
		auto newSence = TransitionFade::create(1.5f,IllustrationSence::scene());
		if (newSence)
		{
			Director::getInstance()->replaceScene(newSence);
		}
	}
}

void VictorySence::setScore(const std::string& jigsawFileName,int useTime,int useStep,int useItem)
{
	_jigsawFileName = jigsawFileName;

	if (useTime > 9999)
	{
		_useTime = 9999;
	}
	else if (useTime < 0)
	{
		_useTime = 0;
	}
	else
	{
		_useTime = useTime;
	}

	if (useStep > 9999)
	{
		_useStep = 9999;
	}
	else if (useStep < 0)
	{
		_useStep = 0;
	}
	else
	{
		_useStep = useStep;
	}

	_useItem = useItem;

	// ���ֹ���
	_totalRollCount = 40;
	_currentRollCount = 0;
	this->schedule(schedule_selector(VictorySence::numberRoll),0.02f,_totalRollCount,0.2f);

	// ������Ԥ���
	auto evaluationClass = getEvaluationClass();
	if (evaluationClass == EvaluationClass::EVALUATION_CLASS_S)
	{
		_evaluationClassSprite = Sprite::create(FileUtils::getInstance()->fullPathForFilename("evaluation_class_s.png"));
	}
	else if (evaluationClass == EvaluationClass::EVALUATION_CLASS_A)
	{
		_evaluationClassSprite = Sprite::create(FileUtils::getInstance()->fullPathForFilename("evaluation_class_a.png"));
	}
	else if (evaluationClass == EvaluationClass::EVALUATION_CLASS_B)
	{
		_evaluationClassSprite = Sprite::create(FileUtils::getInstance()->fullPathForFilename("evaluation_class_b.png"));
	}
	else if (evaluationClass == EvaluationClass::EVALUATION_CLASS_C)
	{
		_evaluationClassSprite = Sprite::create(FileUtils::getInstance()->fullPathForFilename("evaluation_class_c.png"));
	}
	else
	{
		_evaluationClassSprite = nullptr;
	}
	
	if (_evaluationClassSprite)
	{
		_evaluationClassSprite->setVisible(false);
		_evaluationClassSprite->setScale(10.0f);
		_evaluationClassSprite->setPosition(Point(_evaluationClassPanel->getSize().width/2,_evaluationClassPanel->getSize().height/2));
		auto actionSkewTo = SkewTo::create(0.f, 37.2f, -37.2f);
		_evaluationClassSprite->runAction(actionSkewTo);
		_evaluationClassPanel->addChild(_evaluationClassSprite);
	}

	// ������е���Ƭ
	unsigned int photoFragmentMask = PhotoManager::getInstance()->getFragmentMask(_jigsawFileName);

	const int piecesInWidth = 3;
	const int piecesInHeight = 4;
	const int totalFragment = piecesInWidth*piecesInHeight;

	// �����Ƭ�Ѿ����ˣ��Ǿ�����
	if (photoFragmentMask == ((1 << totalFragment) - 1))
	{
		_getFragment = false;
	}
	// ���û�����Ǿ�����û�õ�����Ƭ�����һ��
	else
	{
		// ��һ����Ƭ���͵�
		auto allPhotoFragments = PhotoManager::getInstance()->getAllPhotoFragments();
		if (allPhotoFragments.empty())
		{
			_getFragment = true;
		}
		else
		{
			_getFragment = getFragmentByEvaluation(evaluationClass);
		}
		
		if (_getFragment)
		{
			std::vector<int> ungettedFragmentIndex;

			for (int i = 0 ; i < totalFragment ; ++i)
			{
				if (photoFragmentMask & (1<<i))
				{
					// �����Ѿ���õ���Ƭ
				}
				else
				{
					ungettedFragmentIndex.push_back(i);
				}
			}

			srand((unsigned)time(0));
			int pickedFragment = ungettedFragmentIndex[rand()%ungettedFragmentIndex.size()];

			// ����Ƭ�ŵ�Panel��
			Texture2D *photoTexture = Director::getInstance()->getTextureCache()->addImage(
					FileUtils::getInstance()->fullPathForFilename(jigsawFileName));

			int photoWidth = photoTexture->getPixelsWide();
			int photoHeight = photoTexture->getPixelsHigh();
			int fragmentWidth = photoWidth/piecesInWidth;
			int fragmentHeight = photoHeight/piecesInHeight;
			int indexX = pickedFragment%piecesInWidth;
			int indexY = pickedFragment/piecesInWidth;

			Sprite* photoFragment = Sprite::createWithTexture(photoTexture,Rect(indexX*fragmentWidth,indexY*fragmentHeight,fragmentWidth,fragmentHeight));
			photoFragment->setAnchorPoint(Point(0,0));
			_fragmentDisplayPanel->addChild(photoFragment);

			// ����Ƭ��ӵ��ļ���
			PhotoManager::getInstance()->addFragmentMask(_jigsawFileName,1 << pickedFragment);
		}
	}
}

void VictorySence::numberRoll(float dt)
{
	float rollFactor = (float)_currentRollCount / (float)_totalRollCount;

	// �ɼ�����
	char str[10] = {0};
	sprintf(str,"%d",static_cast<int>(_useTime * rollFactor));
	_useTimeText->setText(str);
	sprintf(str,"%d",static_cast<int>(_useStep * rollFactor));
	_useStepText->setText(str);
	sprintf(str,"%d",static_cast<int>(_useItem * rollFactor));
	_useItemText->setText(str);

	++_currentRollCount;

	if (_currentRollCount == _totalRollCount)
	{
		// ������ʾ
		if (_evaluationClassSprite)
		{
			_evaluationClassSprite->setVisible(true);
			auto actionScaleTo = ScaleTo::create(0.25f,1.0f);
			auto actionSkewTo = SkewTo::create(0.25f, 0.f, 0.f);
			_evaluationClassSprite->runAction(Sequence::create(actionScaleTo
				,DelayTime::create(0.5f)
				,CallFunc::create(CC_CALLBACK_0(VictorySence::setImageAndButtonVisible,this))
				,NULL));
			_evaluationClassSprite->runAction(actionSkewTo);
		}
	}
}

VictorySence::EvaluationClass VictorySence::getEvaluationClass()
{
	// ��������ʱ�䣬���ò�����ʹ�õ��ߴ������������
	if (_useItem == 0)
	{
		if (_useTime < 1800 && _useStep < 800)
		{
			return EvaluationClass::EVALUATION_CLASS_S;
		}
		else if(_useTime < 2400 && _useStep < 1200)
		{
			return EvaluationClass::EVALUATION_CLASS_A;
		}
		else
		{
			return EvaluationClass::EVALUATION_CLASS_B;
		}
	}
	else if (_useItem == 1 || _useItem == 2)
	{
		if (_useTime < 1800 && _useStep < 800)
		{
			return EvaluationClass::EVALUATION_CLASS_A;
		}
		else if(_useTime < 2400 && _useStep < 1200)
		{
			return EvaluationClass::EVALUATION_CLASS_B;
		}
		else
		{
			return EvaluationClass::EVALUATION_CLASS_C;
		}
	}
	else if (_useItem == 3 || _useItem == 4)
	{
		if (_useTime < 1800 && _useStep < 800)
		{
			return EvaluationClass::EVALUATION_CLASS_B;
		}
		else
		{
			return EvaluationClass::EVALUATION_CLASS_C;
		}
	}
	else
	{
		return EvaluationClass::EVALUATION_CLASS_C;
	}
}

bool VictorySence::getFragmentByEvaluation(EvaluationClass evaluationClass)
{
	if (evaluationClass == EvaluationClass::EVALUATION_CLASS_S)
	{
		// ����S 100%�����Ƭ
		return true;
	}
	else if (evaluationClass == EvaluationClass::EVALUATION_CLASS_A)
	{
		// ����A 50%�����Ƭ
		srand((unsigned)time(0));
		return (rand()%100) < 50;
	}
	if (evaluationClass == EvaluationClass::EVALUATION_CLASS_B)
	{
		// ����B 33%�����Ƭ
		srand((unsigned)time(0));
		return (rand()%100) < 33;
	}
	if (evaluationClass == EvaluationClass::EVALUATION_CLASS_C)
	{
		// ����C 10%�����Ƭ
		srand((unsigned)time(0));
		return (rand()%100) < 10;
	}
	else
	{
		return false;
	}
}

void VictorySence::setImageAndButtonVisible()
{
	if (_getFragment)
	{
		_getFragmentImage->setVisible(true);
		_fragmentDisplayPanel->setVisible(true);
		_illustrationButton->setVisible(true);
		_illustrationButton->setTouchEnabled(true);
	}
	else
	{
		_noGetFragmentImage->setVisible(true);
		_playAgainButton->setPositionX(Director::getInstance()->getWinSize().width/2);
	}

	_playAgainButton->setVisible(true);
	_playAgainButton->setTouchEnabled(true);
}

#include <iostream>  
#include <ctime>
#include <cstdlib>  

#include "Map.h"
#include "Global.h"
#include "Snake.h"

using std::cout;
using std::endl;

Snake::Snake()
    : m_length(3)
	, m_foodCoordX(0)
	, m_foodCoordY(0) 
{
    m_belongMap = new Map(25,25);
	if (NULL == m_belongMap) {
		cout << "new m_belongMap error" << endl;
	}
		
	init();
	drawSnake();
	randomFood();		
}

Snake::Snake(int snakeLen, int mapWidth, int mapHeight)
	: m_length(snakeLen)
	, m_foodCoordX(0)
	, m_foodCoordY(0) 
{
	m_belongMap = new Map(mapWidth, mapHeight);
	    if (NULL == m_belongMap) {
		    cout << "new m_belongMap error" << endl;
	    }
	    
	init(); 
    drawSnake();
	randomFood();  
}

Snake::~Snake()
{
	if (NULL != m_belongMap) {
	    delete m_belongMap;
		m_belongMap = NULL;
	}
}

void Snake::init(void)
{
	//�߳����ڵ�ͼ������  
	m_x[0] = m_belongMap->getWidth() / 2 + m_belongMap->getMapStartingCoordX();  
    m_y[0] = m_belongMap->getHeight() / 2 + m_belongMap->getMapStartingCoordY();  
        
    for (int i = 1; i < m_length; i++) {  
        m_x[i] = m_x[0];  //��������������ͷ���� 
        m_y[i] = m_y[i - 1] - 1; //����������Ϊ��һ�ڼ�1 
    } 	
}
//���� 
void Snake::drawSnake()
{  
    Display::setColor(10);  
    for (int i = 0; i < m_length; i++) {  
        Display::gotoXY(m_x[i], m_y[i]);  
        cout << "��" << endl;  
    }  
}

//�������ʳ��
void Snake::randomFood()           
{
	srand((unsigned)time(NULL));
	do {
		//ʳ�ﲻ�ܲ������ı� 
        m_foodCoordX = rand() % (m_belongMap->getWidth() - 2) + 1 + m_belongMap->getMapStartingCoordX(); 
        m_foodCoordY = rand() % (m_belongMap->getHeight() - 2) + 1 + m_belongMap->getMapStartingCoordY();
        
        //ʳ�ﲻ�ܲ������ߵ������� 
       	bool part = isPartOfBody();  
        if (false == part) {
			break;
		}
	} while(1);
	  
    Display::gotoXY(m_foodCoordX, m_foodCoordY);
	Display::setColor(12);
	cout << "��" << endl;	
}

//�жϲ�����ʳ���Ƿ����ߵ������� 
bool Snake::isPartOfBody(void)       
{
	bool part = false;
	
	for (int i = 0; i < m_length; i++) {     
        if (m_foodCoordX == m_x[i] && m_foodCoordY == m_y[i]) { 
        	part = true;
			break;
	    } 
    }
	return part;
}

void Snake::snakeMove(char dir) 
{
	//��β����䷽��
	Display::gotoXY(m_x[m_length - 1], m_y[m_length - 1]);  
    Display::setColor(19);  
    cout << "��"; 
 
	//�ߵ��ƶ��㷨, ��ǰһ�ڵ����긳����һ��
    for (int i = m_length - 1; i > 0; i--) {  
        m_x[i] = m_x[i - 1];   
        m_y[i] = m_y[i - 1];  
    }
    
    //���ݷ����ƶ��ߵ����� 
	switch (dir) {
	case UP:
	  {
		m_y[0] = m_y[0] - 1;//��ͷ�������1
	  }  
	  break;
	case DOWN:
	  {
		m_y[0] = m_y[0] + 1;//��ͷ�������1
	  }  
	  break;
	case LEFT:
	  {
		m_x[0] = m_x[0] - 1;//��ͷ�������1
	  }  
	  break;
	case RIGHT:
	  {
		m_x[0] = m_x[0] + 1;//��ͷ�������1
	  }  
	  break;
	default:
      {
	  }
	  break; 
	}
}

//�����ƶ�   
void Snake::moveUp()  
{
    snakeMove(UP);   
    drawSnake();  
}  

//�����ƶ�  
void Snake::moveDown()  
{  
    snakeMove(DOWN); 
    drawSnake();  
}  

//�����ƶ�  
void Snake::moveLeft()  
{
    snakeMove(LEFT); 
    drawSnake();  
}  

//�����ƶ�  
void Snake::moveRight()  
{  
    snakeMove(RIGHT); 
    drawSnake();  
}

//�ж��Ƿ�Ե�ʳ��
bool Snake::eatFood()  
{  
    if ((m_x[0] == m_foodCoordX) && (m_y[0] == m_foodCoordY)) {    
        m_x[m_length] = m_x[m_length-1];//�������ӵ���ƨ��������ڽ�ʳǰ����ƨ�ɣ��ƶ�����Զ�ˢ�� 
		m_y[m_length] = m_y[m_length-1];
		
		m_length += 1;
		
        randomFood();  //����֮�����ʳ��
		 
        return true;  
    } 
	 
    return false;  
}   

//�ж����Ƿ������߿�����Լ� 
bool Snake::getLifeStatus(void) 
{
  	//�ж��Ƿ�������ͼ�߿� 
	bool lifeStatus = true;
	bool collisionStatus = false; 
	
    collisionStatus = isCollisionMapBorder();
    if (collisionStatus) {
    	lifeStatus = false;
    	return lifeStatus;
	}
	
	collisionStatus = isCollisionPartOfBody();
    if (collisionStatus) {
    	lifeStatus = false;
	}
	
    return lifeStatus;	  
}  

bool Snake::isCollisionMapBorder(void) 
{
	bool collisionStatus = false; 
	
    if ((m_x[0] <= m_belongMap->getMapStartingCoordX())       
        || (m_x[0] >= m_belongMap->getWidth() + m_belongMap->getMapStartingCoordX() - 1) 
        || (m_y[0] <= m_belongMap->getMapStartingCoordY()) 
        || (m_y[0] >= m_belongMap->getHeight() + m_belongMap->getMapStartingCoordY() - 1)) {
            
			collisionStatus = true;
		} 
		
	return collisionStatus;
}

bool Snake::isCollisionPartOfBody(void) 
{
	bool collisionStatus =false; 
          
    //�ж��Ƿ������Լ�,�ӵ�4�ڿ�ʼ��,��Ϊͷ�϶���������2/3��
    for (int i = 3; i < m_length; i++) {       
        if (m_x[0] == m_x[i] && m_y[0] == m_y[i]) {
        	collisionStatus = true;	
        	break;
		}           
    }
		
	return collisionStatus;
}



/*
//�ж����Ƿ������߿�����Լ� 
bool Snake::getLifeStatus(void) {
	
  	//�ж��Ƿ�������ͼ�߿� 
	int lifeStatus = true; 
    if (m_x[0] <= m_belongMap->getMapStartingCoordX()         
        || m_x[0] >= m_belongMap->getWidth() + m_belongMap->getMapStartingCoordX() - 1  
        || m_y[0] <= m_belongMap->getMapStartingCoordY()  
        || m_y[0] >= m_belongMap->getHeight() + m_belongMap->getMapStartingCoordY() - 1) {
            
			lifeStatus = false;
			 
			return lifeStatus;
		} 
    
	//�ж��Ƿ������Լ�,�ӵ�4�ڿ�ʼ��,��Ϊͷ�϶���������2/3��      
    int i = 0;
    for (i = 3; i < m_length; i++) {       
        if (m_x[0] == m_x[i] && m_y[0] == m_y[i]) {
        	lifeStatus = false;	
        	break;
		}           
    }
    return lifeStatus;	  
} 
*/ 

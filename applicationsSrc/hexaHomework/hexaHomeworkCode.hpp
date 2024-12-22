
#ifndef hexaHomeworkCode_H_
#define hexaHomeworkCode_H_

#include "robots/hexanodes/hexanodesSimulator.h"
#include "robots/hexanodes/hexanodesWorld.h"
#include "robots/hexanodes/hexanodesBlockCode.h"
#include "robots/hexanodes/hexanodesMotionEvents.h"
#include "robots/hexanodes/hexanodesMotionEngine.h"
static const int GO_MSG_ID = 1001;
static const int BACK_MSG_ID = 1002;
static const int MOVE_MSG_ID = 1003;


using namespace Hexanodes;

class HexaHomeworkCode : public HexanodesBlockCode {
private:
	HexanodesBlock *module = nullptr;
	bool isLeader=false;
	int distance;
	P2PNetworkInterface *parent = nullptr;
    	int waitingMessage = 0;
    	pair<int, int> farthestNode;
    	bool receivedMove=false;
    	Cell3DPosition initialPosition;
    	bool isClockwise=true;
    	int nbNeighbors=-1;

public :
	inline static size_t motionCounter = 0;
	HexaHomeworkCode(HexanodesBlock *host);
	~HexaHomeworkCode() {};

/**
  * This function is called on startup of the blockCode, it can be used to perform initial
  *  configuration of the host or this instance of the program.
  * @note this can be thought of as the main function of the module
  */
    void startup() override;

/**
  * @brief Message handler for the message 'go'
  * @param _msg Pointer to the message received by the module, requires casting
  * @param sender Connector of the module that has received the message and that is connected to the sender
  */
   void myGoFunc(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);

/**
  * @brief Message handler for the message 'back'
  * @param _msg Pointer to the message received by the module, requires casting
  * @param sender Connector of the module that has received the message and that is connected to the sender
  */
   void myBackFunc(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);

/**
  * @brief Message handler for the message 'move'
  * @param _msg Pointer to the message received by the module, requires casting
  * @param sender Connector of the module that has received the message and that is connected to the sender
  */
   void myMoveFunc(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);

/**
  * @brief Provides the user with a pointer to the configuration file parser, which can be used to read additional user information from each block config. Has to be overridden in the child class.
  * @param config : pointer to the TiXmlElement representing the block configuration file, all information related to concerned block have already been parsed
  *
  */
    void parseUserBlockElements(TiXmlElement *config) override;

/**
  * @brief Callback function executed whenever the module finishes a motion
  */
    void onMotionEnd() override;
    bool isInTarget();
    bool isPositionInTarget(Cell3DPosition position);

/**
  * Called by openglviewer during interface drawing phase, can be used by a user
  *  to draw a custom Gl string onto the bottom-left corner of the GUI
  * @note call is made from OpenGlViewer::drawFunc
  * @return a string (can be multi-line with `
`) to display on the GUI
  */
    string onInterfaceDraw() override;
    pair<bool,Cell3DPosition> isConnector();

/*****************************************************************************/
/** needed to associate code to module                                      **/
	static BlockCode *buildNewBlockCode(BuildingBlock *host) {
	    return(new HexaHomeworkCode((HexanodesBlock*)host));
	}
/*****************************************************************************/
};

#endif /* hexaHomeworkCode_H_ */

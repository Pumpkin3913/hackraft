#include "player.h"

#include "screen.h"

#ifdef __linux__
#include <unistd.h>
#elif defined _WIN32
#endif

// PRIVATE

void Player::send(std::string message) {
	std::string toSend = message + "\n";
#ifdef __linux__
	write(this->fd, toSend.c_str(), toSend.length());
	// this->socket << message << std::endl;
#elif defined _WIN32
#endif
}

std::string Player::receive() {
	std::string msg = "";
#ifdef __linux__
	char c;
	int flag;
	flag = read(this->fd, &c, 1);
	while(flag && c != '\n') {
		msg.push_back(c);
		flag = read(this->fd, &c, 1);
	}
	if(!flag) {
		this->stop = true;
	}
	/* XXX //
	for(read(this->fd, &c, 1); c != '\n'; read(this->fd, &c, 1)) {
		msg.push_back(c);
	}
	// XXX */
#elif defined _WIN32
#endif
	return(msg);
}

void Player::_close() {
	this->send("EOF");
	this->stop = true;
#ifdef __linux__
	close(this->fd);
#elif defined _WIN32
#endif
}

void Player::loopFunction() {
	while(!this->stop) {
		this->parse();
	}
	this->loopThread = NULL;
	delete(this);
}

void Player::parse() {
	std::string msg;
	std::string cmd;
	std::string arg;
	std::size_t separator;

	msg = this->receive();
	separator = msg.find_first_of(' ');
	if(separator == std::string::npos) {
		cmd = msg;
		arg = "";
	} else {
		cmd = msg.substr(0, separator);
		arg = msg.substr(separator+1); // from separator+1 to the end.
	}

	verbose_info("Player command '"+cmd+"' with args '"+arg+"'."); // XXX

	if(!cmd.compare("move")) {
		signed int xShift = 0;
		signed int yShift = 0;
		if(!arg.compare("north")) {
			yShift--;
		} else if(!arg.compare("south")) {
			yShift++;
		} else if(!arg.compare("west")) {
			xShift--;
		} else if(!arg.compare("east")) {
			xShift++;
		}
		this->move(xShift, yShift);
	} else if(!cmd.compare("say")) {
		this->screen->mutex.lock();
		this->screen->event(this->name+" say "+arg);
		this->screen->mutex.unlock();
	} else if(!cmd.compare("quit")) {
		this->stop = true;
	}
}

// PUBLIC

Player::Player(int fd, std::string name, std::string description, Aspect aspect) :
	fd(fd),
	name(name),
	description(description),
	aspect(aspect),
	screen(NULL),
	x(0),
	y(0),
	/* XXX //
	movepoints(0),
	visible(true),
	solid(true),
	movable(true),
	// XXX */
	loopThread(NULL),
	stop(false)
{ }

Player::~Player() {
// ToDO : latter : ~Player() : delete objects, gauges and tags.
// ToDO : latter : trigger this->onDeath here ?
	if(this->screen) {
		this->screen->exitPlayer(this);
	}
	if(this->fd) {
		this->_close();
	}
	if(this->loopThread) {
		// Only when deleted by something else than its self loopThread.
		this->loopThread->detach();
		delete(this->loopThread);
	}
}

void Player::spawn(class Screen * screen, int x, int y) {
	if(!this->loopThread) {
		this->screen = screen;
		this->x = x;
		this->y = y;
		this->screen->mutex.lock();
		this->screen->enterPlayer(this);
		this->screen->mutex.unlock();
		this->loopThread = new std::thread(&Player::loopFunction, this);
	}
}

int Player::getId() {
	return(this->fd);
}

std::string Player::getName() {
	return(this->name);
}

// TODO : Player::setName() : broadcast new name.
void Player::setName(std::string name) {
	this->name = name;
}

std::string Player::getDescription() {
	return(this->description);
}

void Player::setDescription(std::string description) {
	this->description = description;
}

Aspect Player::getAspect() {
	return(this->aspect);
}

// TODO : Player::setAspect() : auto broadcast new aspect.
void Player::setAspect(Aspect aspect) {
	this->aspect = aspect;
}

class Screen * Player::getScreen() {
	return(this->screen);
}

unsigned int Player::getX() {
	return(this->x);
}

unsigned int Player::getY() {
	return(this->y);
}

void Player::setXY(int x, int y) {
	this->x = x;
	this->y = y;
	if(this->screen) {
		this->screen->mutex.lock();
		this->screen->updatePlayerPosition(this);
		this->screen->mutex.unlock();
	}
}

void Player::move(int xShift, int yShift) {
	int new_x = this->x + xShift;
	int new_y = this->y + yShift;
	if(this->screen) {
		if(this->screen->canLandPlayer(this, new_x, new_y)) {
			this->setXY(new_x, new_y);
		}
	}
}

void Player::changeScreen(class Screen * newScreen, int x, int y) {
	if(this->screen) {
		this->screen->exitPlayer(this);
		this->x = x;
		this->y = y;
		this->screen = newScreen;
		this->screen->enterPlayer(this);
	}
}

// getObject();
// addObject();
// remObject();
// getGauge();
// addGauge();
// delGauge();
// getTag();
// addTag();
// delTag();

/* XXX //
unsigned int Player::getMovePoints() {
	return(this->movepoints);
}

void Player::setMovePoints(unsigned int points) {
	this->movepoints = points;
}

void Player::resetMovePoints() {
	this->movepoints = 0;
}

bool Player::isVisible() {
	return(this->visible);
}

void Player::setVisible() {
	this->visible = true;
}

void Player::setNotVisible() {
	this->visible = false;
}

bool Player::isSolid() {
	return(this->solid);
}

void Player::setSolid() {
	this->solid = true;
}

void Player::setNotSolid() {
	this->solid = false;
}

bool Player::isMovable() {
	return(this->movable);
}

void Player::setMovable() {
	this->movable = true;
}

void Player::setNotMovable() {
	this->movable = false;
}
// XXX */

/* Send messages to client */

void Player::message(std::string message) {
	this->send("msg " + message);
}

void Player::updatePlayer(class Player * player) {
	// move <plrID> <X> <Y>
	this->send(
			"move "
			+ std::to_string(player->getId())
			+ " "
			+ std::to_string(player->getX())
			+ " "
			+ std::to_string(player->getY())
		  );
}

void Player::updatePlayerExit(class Player * player) {
	this->send("exit "+std::to_string(player->getId()));
}

void Player::updateFloor() {
	// floor <W> <H> <name>
	this->send(
			"floor "
			+ std::to_string(this->screen->getWidth())
			+ " "
			+ std::to_string(this->screen->getHeight())
			+ " "
			+ this->screen->getName()
		  );

	// Send tiles.
	std::string toSend = "";
	for(unsigned int y=0; y<this->screen->getHeight(); y++) {
		for(unsigned int x=0; x<this->screen->getWidth(); x++) {
			toSend += std::to_string(this->screen->getPlace(x, y)->getAspect());
			toSend += ",";
		}
	}
	toSend.pop_back();
	this->send(toSend);
}

void Player::updateTile(unsigned int x, unsigned int y, Aspect aspect) {
	// tilechange <aspect> <X> <Y>
	this->send(
			"tilechange "
			+ std::to_string(aspect)
			+ " "
			+ std::to_string(x)
			+ " "
			+ std::to_string(y)
		  );
}

void Player::updateObject(unsigned int x, unsigned int y, Aspect aspect) {
	// obj <aspect> <X> <Y>
	this->send(
			"obj "
			+ std::to_string(aspect)
			+ " "
			+ std::to_string(x)
			+ " "
			+ std::to_string(y)
		  );
}

void Player::updateNoObject(unsigned int x, unsigned int y) {
	// noobj <X> <Y>
	this->send(
			"noobj "
			+ std::to_string(x)
			+ " "
			+ std::to_string(y)
		  );
}


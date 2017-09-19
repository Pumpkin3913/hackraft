#include "player.h"

#include "character.h"
#include "server.h"
#include "zone.h"
#include "place.h"
#include "log.h"

#include <unistd.h>

// PUBLIC

Player::Player(int fd, class Character* character) :
	fd(fd),
	character(character)
{ }

Player::~Player() {
	info("Player "+std::to_string(this->fd)+" deleted.");
	close(this->fd);
	this->character->setPlayer(nullptr);
	// For now, the disconnection of a player kills the character. Later, a reconnection feature might be interesting.
	this->character->getZone()->getServer()->delCharacter(this->character->getId());
}

void Player::check_action() {
	std::string msg = this->receive();
	if(msg != "") {
		this->parse(msg);
	}
}

bool Player::delme() {
	return(this->_delme);
}

void Player::message(std::string message) {
	this->send("msg " + message);
}

void Player::updateCharacter(class Character * character) {
	// move <plrID> <X> <Y>
	this->send(
			"move "
			+ character->getId().toString()
			+ " "
			+ std::to_string(Aspect::getAspectEntry(character->getAspect()))
			+ " "
			+ std::to_string(character->getX())
			+ " "
			+ std::to_string(character->getY())
		  );
}

void Player::updateCharacterExit(class Character * character) {
	this->send("exit "+character->getId().toString());
}

void Player::updateFloor() {
	this->send("zonename " + this->character->getZone()->getName().toString());
	// floor <W> <H> <name>
	this->send(
			"zone "
			+ std::to_string(this->character->getZone()->getWidth())
			+ " "
			+ std::to_string(this->character->getZone()->getHeight())
			+ " "
			+ this->character->getZone()->getName().toString()
		  );

	// Send places' aspects.
	std::string toSend = "";
	for(unsigned int y=0; y<this->character->getZone()->getHeight(); y++) {
		for(unsigned int x=0; x<this->character->getZone()->getWidth(); x++) {
			toSend += std::to_string(this->character->getZone()->getPlace(x, y)->getAspect().toEntry());
			toSend += ",";
		}
	}
	toSend.pop_back();
	this->send(toSend);
}

void Player::updateFloor(unsigned int x, unsigned int y, const Aspect& aspect) {
	// floorchange <aspect> <X> <Y>
	this->send(
			"floorchange "
			+ std::to_string(aspect.toEntry())
			+ " "
			+ std::to_string(x)
			+ " "
			+ std::to_string(y)
		  );
}

void Player::updateGauge(
	std::string name,
	unsigned int val,
	unsigned int max,
	const Aspect& full,
	const Aspect& empty
) {
	// gauge <name> <val> <max> <full> <empty>
	this->send(
			"gauge "
			+ name
			+ " "
			+ std::to_string(val)
			+ " "
			+ std::to_string(max)
			+ " "
			+ std::to_string(Aspect::getAspectEntry(full))
			+ " "
			+ std::to_string(Aspect::getAspectEntry(empty))
	);
}

void Player::updateNoGauge(std::string name) {
	this->send(
			"nogauge "
			+ name
	);
}

/* XXX //
void Player::updateInventory(unsigned long int id, Aspect aspect) {
	// invent <id> <aspect>
	this->send(
			"invent "
			+ std::to_string(id)
			+ " "
			+ std::to_string(aspect)
	);
}

void Player::updateNoInventory(unsigned long int id) {
	// noinvent <id>
	this->send("noinvent " + std::to_string(id));
}

void Player::addPickupList(unsigned long int id, Aspect aspect) {
	// pickuplist <id> <aspect>
	this->send(
			"addpickuplist "
			+ std::to_string(id)
			+ " "
			+ std::to_string(aspect)
	);
}

void Player::remPickupList(unsigned long int id) {
	// pickuplist <id> <aspect>
	this->send(
			"rempickuplist "
			+ std::to_string(id)
	);
}
// XXX */

void Player::follow(class Character * character) {
	this->send("follow " + character->getId().toString());
}

void Player::hint(Aspect aspect, std::string hint) {
	this->send("hint "
		+ std::to_string(aspect.toEntry())
		+ " "
		+ hint
	);
}

// PRIVATE

void Player::send(std::string message) {
	if(this->fd) {
		std::string toSend = message + "\n";
		write(this->fd, toSend.c_str(), toSend.length());
		// this->socket << message << std::endl;
	}
}

std::string Player::receive() {
	std::string msg = "";
	char c;
	int flag;
	flag = read(this->fd, &c, 1);
	while(flag > 0 and c != '\n') {
		msg.push_back(c);
		flag = read(this->fd, &c, 1);
	}
	// EAGAIN and EWOULDBLOCK are "errors" when nothing is available on a non-blocking socket.
	if(flag == 0 or (flag == -1 and errno != EAGAIN and errno != EWOULDBLOCK)) {
		this->_delme = true;
	}
	return(msg);
}

void Player::parse(std::string msg) {
	std::string cmd;
	std::string arg;
	std::size_t separator;

	separator = msg.find_first_of(' ');
	if(separator == std::string::npos) {
		cmd = msg;
		arg = "";
	} else {
		cmd = msg.substr(0, separator);
		arg = msg.substr(separator+1); // from separator+1 to the end.
	}

	if(cmd[0] == '/') {
		if(this->character->getZone()) {
			this->character->getZone()->getServer()->doAction(cmd.substr(1), *this->character, arg);
		}
	} else if(cmd == "move") {
		signed int xShift = 0;
		signed int yShift = 0;
		bool valid = true;
		if(arg == "north") {
			yShift--;
		} else if(arg == "south") {
			yShift++;
		} else if(arg == "west") {
			xShift--;
		} else if(arg == "east") {
			xShift++;
		} else {
			valid = false;
		}
		if(valid) {
			this->character->move(xShift, yShift);
		}
	} else if(cmd == "say") {
		if(this->character->getZone()) {
			this->character->getZone()->event(this->character->getName().toString()+": "+arg);
		}
	} else if(cmd == "quit") {
		this->_delme = true;
	}
}

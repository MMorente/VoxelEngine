#pragma once

enum class Blocktype {
	Default,
	Grass,
	Dirt,
	Water,
	Stone,
	Wood,
	Sand,
};

class Block {
public:
	Block(){}
	~Block(){}
	bool isActive() { return active; }
	void setActive(bool status) { active = status; }
private:
	bool active = false;
	Blocktype blockType = Blocktype::Default;
};


// SnakeTheGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include <Windows.h>
#include <stack>

using namespace std;

struct Snake {
	int headPos;
	int headVel;
	int tailPos;
	int tailVel;
	stack<char>* headStack;
};

struct GameFieldCellValuesConstants {
	/*
	Взаимоисключающиеся состояния (активные)
	*/
	static const char EMPTY_CELL = 0x01;
	static const char CHEESE_CELL = 0x02;
	static const char SNAKE_CELL = 0x04;
	
	//Впринципе их можно не совать в одну переменную а сделать 2 отдельных но кого это ебет

	/**
	 Cтатичные не взаимоисключающиеся (пассивные)
	**/
	static const char LEFT_BOARD_CELL = 0x10;
	static const char RIGHT_BOARD_CELL = 0x20;
	static const char UP_BOARD_CELL = 0x40;
	static const char DOWN_BOARD_CELL = 0x80;
};

struct GameField {
	char* fieldArray;
	char fieldDimension;
	int fieldArrayLength;
	char* fieldGraphicsRepresentation;
	int graphicsRepresentaionArrayLength;
	int graphicsRepresentationDimension;
};

struct GameData {
	GameField* gameFieldModel;
	Snake* snakeModel;
	map<char, char>* charMap;
	map<char, void (*)(GameData*)>* eventMap;
};

int logicToGraphicsMapper(int logicCoords, int fieldDimension, int gd) {
	return ((logicCoords / fieldDimension) * gd + (logicCoords << 1)) + gd + (logicCoords / fieldDimension) + 1;
}

void setCellValue(GameField* field, int cellPosition, char newCellValue, map<char, char>* characterMap) {
	field->fieldArray[cellPosition] &= 0xF0;
	field->fieldArray[cellPosition] |= newCellValue;	
	int graphCoord = logicToGraphicsMapper(cellPosition, field->fieldDimension, field->graphicsRepresentationDimension);
	if(characterMap != nullptr && characterMap->count(newCellValue & 0x0F) != 0)
		field->fieldGraphicsRepresentation[graphCoord] =
		characterMap->find(newCellValue & 0x0F)->second;
}

GameField* initGameField(char fieldDimension) {
	GameField* field = new GameField;
	field->fieldDimension = fieldDimension;
	int fieldArrayLength = fieldDimension * fieldDimension;
	
	field->fieldArray = new char[fieldArrayLength];
	field->fieldArrayLength = fieldArrayLength;
	
	for (int i = 0; i < fieldArrayLength; i++) {
		if (i % fieldDimension == 0) field->fieldArray[i] |= GameFieldCellValuesConstants::LEFT_BOARD_CELL;
		if (i < fieldDimension) field->fieldArray[i] |= GameFieldCellValuesConstants::UP_BOARD_CELL;
		if (i >= fieldArrayLength - fieldDimension) field->fieldArray[i] |= GameFieldCellValuesConstants::DOWN_BOARD_CELL;
		if ((i + 1) % fieldDimension == 0) field->fieldArray[i] |= GameFieldCellValuesConstants::RIGHT_BOARD_CELL;
		field->fieldArray[i] |= GameFieldCellValuesConstants::EMPTY_CELL;
	}
	
	int graphicsArrayDimension = (int)((fieldDimension << 1) + 1);// + 1 - для \n
	int fieldGraphicsRepresentaionArrayLength = graphicsArrayDimension * graphicsArrayDimension;
	
	field->fieldGraphicsRepresentation = new char[fieldGraphicsRepresentaionArrayLength];
	field->graphicsRepresentationDimension = graphicsArrayDimension;

	for (int i = 0; i < graphicsArrayDimension; i++) {
		if (i % 2 == 0) {
			for (int j = 0; j < graphicsArrayDimension; j++) {
				field->fieldGraphicsRepresentation[i * graphicsArrayDimension + j] = '-';
				if ((i * graphicsArrayDimension + j) % graphicsArrayDimension == 0) field->fieldGraphicsRepresentation[i * graphicsArrayDimension + j] = '\n';
			}
		}
		else {
			for (int j = 0; j < graphicsArrayDimension; j++) {
				if (j % 2 == 0) field->fieldGraphicsRepresentation[i * graphicsArrayDimension + j] = '|';
				else {
					field->fieldGraphicsRepresentation[i * graphicsArrayDimension + j] = ' ';
				}
				if((i * graphicsArrayDimension + j) % graphicsArrayDimension == 0)field->fieldGraphicsRepresentation[i * graphicsArrayDimension + j] = '\n';
			}
		}
		
	}

	field->graphicsRepresentaionArrayLength = fieldGraphicsRepresentaionArrayLength;

	return field;
}

map<char, char>* initCharacterMap() {
	map<char, char>* characterMap = new map<char, char>();
	characterMap->insert(std::pair<char,char>(GameFieldCellValuesConstants::EMPTY_CELL, ' '));
	characterMap->insert(std::pair<char, char>(GameFieldCellValuesConstants::SNAKE_CELL, '*'));
	characterMap->insert(std::pair<char, char>(GameFieldCellValuesConstants::CHEESE_CELL, 'C'));
	return characterMap;
}


void emptyFieldAction(GameData* gd) {

}

void cheeseAction(GameData* gd) {

}

void leftBoardAction(GameData* gd) {

}

void rightBoardAction(GameData* gd) {

}

void upBoardAction(GameData* gd) {

}

void downBoardAction(GameData* gd) {

}

map<char, void(*)(GameData*)>* initEventMap() {
	map<char, void(*)(GameData*)>* eventMap = new map<char, void(*)(GameData*)>();
	eventMap->insert(std::pair<char, void(*)(GameData*)>(GameFieldCellValuesConstants::CHEESE_CELL, &cheeseAction));
	eventMap->insert(std::pair<char, void(*)(GameData*)>(GameFieldCellValuesConstants::EMPTY_CELL, &emptyFieldAction));
	eventMap->insert(std::pair<char, void(*)(GameData*)>(GameFieldCellValuesConstants::SNAKE_CELL, &cheeseAction));
	eventMap->insert(std::pair<char, void(*)(GameData*)>(GameFieldCellValuesConstants::LEFT_BOARD_CELL, &leftBoardAction));
	eventMap->insert(std::pair<char, void(*)(GameData*)>(GameFieldCellValuesConstants::RIGHT_BOARD_CELL, &rightBoardAction));
	return eventMap;
}

Snake* initSnake() {
	Snake* snake = new Snake();
	snake->headPos = -1;
	snake->headVel = 1;
	snake->tailPos = -1;
	snake->tailVel = 1;
	snake->headStack = new stack<char>();
	return snake;
}

GameData* initGameData() {
	GameData* gd = new GameData();
	gd->gameFieldModel = initGameField(8);
	gd->snakeModel = initSnake();
	gd->charMap = initCharacterMap();
	gd->eventMap = initEventMap();
	setCellValue(gd->gameFieldModel, 0, GameFieldCellValuesConstants::SNAKE_CELL,
		gd->charMap);//Костылек, надо рандом юзать
	setCellValue(gd->gameFieldModel, gd->gameFieldModel->fieldDimension * 1, GameFieldCellValuesConstants::CHEESE_CELL,
		gd->charMap);//Костылек, надо рандом юзать
	return gd;
}

void deallocateResources(GameData* gameData) {
	delete[] gameData->gameFieldModel;
	delete gameData->snakeModel;
	delete gameData->charMap;
	delete gameData->eventMap;
	delete gameData;
}

const char* DRAW_MARGIN = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

void drawGameField(GameField* gameField) {
	for (int i = 0; i < gameField->graphicsRepresentaionArrayLength; i++)
		cout << gameField->fieldGraphicsRepresentation[i];
	cout << DRAW_MARGIN;
}

void update(GameData* gameData) {
	GameField* gfield = gameData->gameFieldModel;
	Snake* snake = gameData->snakeModel;
	
	if (snake->tailPos < 0) {
		snake->headStack->push(GameFieldCellValuesConstants::EMPTY_CELL);
	}
	else {
		snake->headStack->push(gfield->fieldArray[snake->tailPos - snake->tailVel]);
	}
	if (!snake->headStack->empty()) {
		char releaesedChar = snake->headStack->top();
		
		snake->headStack->pop();
		
		setCellValue(gfield, snake->tailPos, releaesedChar & 0x0F, gameData->charMap);
	}

	snake->tailPos += snake->tailVel;
	snake->headPos += snake->headVel;
	setCellValue(gfield, snake->headPos, GameFieldCellValuesConstants::SNAKE_CELL, gameData->charMap);

	if((gameData->eventMap->count(gfield->fieldArray[snake->headPos]) != 0))
		(gameData->eventMap->find(gfield->fieldArray[snake->headPos])->second) (gameData);
}

void startGameProcess() {
	GameData* gameData = initGameData();
	GameField* gameFieldModel = gameData->gameFieldModel;
	
	while (true) {
		update(gameData);
		drawGameField(gameFieldModel);
		Sleep(1000);
	}

	deallocateResources(gameData);
}

int main()
{
	startGameProcess();
    return 0;
}



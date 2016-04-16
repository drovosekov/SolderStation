#ifndef TYPES
#define TYPES

#ifndef false
#define false	0
#define true	!false
#endif

enum EncoderModes{
	selSolderTemperature = 0,
	selAirFlowPower,
	selAirHeaterTemperature
};

enum SolderingStates{
	notReady = 0,	//вне подставки - не готов
	isOff,			//выключен
	isOn,			//включен
	isSleepMode,	//экономный режим ожидания
	isPreOn			//на подставке - готов к включению. Включится при снятии с подставки
};

typedef enum SolderingStates SolderingStates;

#endif

#ifndef TYPES
#define TYPES

#ifndef false
#define false	0
#define true	!false
#endif

enum EncoderModes{
	selRealTimeInfo = 0,
	selSolderTemperature,
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

typedef struct SLD_INFO{
	u16 auto_off;
	u16 temp;
	SolderingStates state;
	u8 air_flow;
}SLD_INFO;

#endif

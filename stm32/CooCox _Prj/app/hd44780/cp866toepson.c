//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Ёто программное обеспечение распростран€етс€ свободно. ¬ы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Ёлектроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// јвтор: Ќадыршин –услан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "cp866toepson.h"

#ifdef CP866
//==============================================================================
// ѕроцедура подмен€ет символы кириллицы в кодировке 866 на символы в кодироке 
// EPSON дл€ hd44780 
//==============================================================================
void cp866_to_epson_convert(char *str)
{
#if (SOURCE_CODEPAGE == CP866)
  while (*str != '\0')
  {
    switch (*str)
    {
    case 'А': *str = 0x41; break;      // А
    case 'Б': *str = 0xA0; break;      // Б
    case 'В': *str = 0x42; break;      // В
    case 'Г': *str = 0xA1; break;      // Г
    case 'Д': *str = 0xE0; break;      // Д
    case 'Е': *str = 0x45; break;      // Е
    case 'р': *str = 0xA2; break;      // р
    case 'Ж': *str = 0xA3; break;      // Ж
    case 'З': *str = 0x33; break;      // З
    case 'И': *str = 0xA5; break;      // И
    case 'Й': *str = 0xA6; break;      // Й
    case 'К': *str = 0x4B; break;      // К
    case 'Л': *str = 0xA7; break;      // Л
    case 'М': *str = 0x4D; break;      // М
    case 'Н': *str = 0x48; break;      // Н
    case 'О': *str = 0x4F; break;      // О
    case 'П': *str = 0xA8; break;      // П
    case 'Р': *str = 0x50; break;      // Р
    case 'С': *str = 0x43; break;      // С
    case 'Т': *str = 0x54; break;      // Т
    case 'У': *str = 0xA9; break;      // У
    case 'Ф': *str = 0xAA; break;      // Ф
    case 'Х': *str = 0x58; break;      // Х
    case 'Ц': *str = 0xE1; break;      // Ц
    case 'Ч': *str = 0xAB; break;      // Ч
    case '?': *str = 0xAC; break;      // ?
    case 'Щ': *str = 0xE2; break;      // Щ
    case 'Ъ': *str = 0xAD; break;      // Ъ
    case 'Ы': *str = 0xAE; break;      // Ы
    case 'Ь': *str = 0x62; break;      // Ь
    case 'Э': *str = 0xAF; break;      // Э
    case 'Ю': *str = 0xB0; break;      // Ю
    case 'Я': *str = 0xB1; break;      // Я
    case '†': *str = 0x61; break;      // †
    case '°': *str = 0xB2; break;      // °
    case 'Ґ': *str = 0xB3; break;      // Ґ
    case '£': *str = 0xB4; break;      // £
    case '§': *str = 0xE3; break;      // §
    case '•': *str = 0x65; break;      // •
    case 'с': *str = 0xB5; break;      // с
    case '¶': *str = 0xB6; break;      // ¶
    case 'І': *str = 0xB7; break;      // І
    case '®': *str = 0xB8; break;      // ®
    case '©': *str = 0xB9; break;      // ©
    case '™': *str = 0xBA; break;      // ™
    case 'Ђ': *str = 0xBB; break;      // Ђ
    case 'ђ': *str = 0xBC; break;      // ђ
    case '≠': *str = 0xBD; break;      // ≠
    case 'Ѓ': *str = 0x6F; break;      // Ѓ
    case 'ѓ': *str = 0xBE; break;      // ѓ
    case 'а': *str = 0x70; break;      // а
    case 'б': *str = 0x63; break;      // б
    case 'в': *str = 0xBF; break;      // в
    case 'г': *str = 0x79; break;      // г
    case 'д': *str = 0xE4; break;      // д
    case 'е': *str = 0x78; break;      // е
    case 'ж': *str = 0xE5; break;      // ж
    case 'з': *str = 0xC0; break;      // з
    case 'и': *str = 0xC1; break;      // и
    case 'й': *str = 0xE6; break;      // й
    case 'к': *str = 0xC2; break;      // к
    case 'л': *str = 0xC3; break;      // л
    case 'м': *str = 0xC4; break;      // м
    case 'н': *str = 0xC5; break;      // н
    case 'о': *str = 0xC6; break;      // о
    case 'п': *str = 0xC7; break;      // п
    }
    str++;
  }
  #endif
}
//==============================================================================
#endif


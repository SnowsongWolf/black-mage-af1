// Light animation header for Black Mage v1 software
// © Zack "Snow" Podany 2016

const int STATE_NULL       = -1;
const int STATE_IGNITE     = 1;
const int STATE_EXTINGUISH = 0;
const int STATE_SMALLCAST  = 4;
const int STATE_BIGCAST    = 6;
const int STATE_LIMITCAST  = 8;
const int STATE_IDLE       = 10;
const int STATE_OFF        = 11;

const int ACT_NULL         = -1;
const int ACT_IGNITE       = 1;
const int ACT_EXTINGUISH   = 0;
const int ACT_SMALLSPELL   = 5;
const int ACT_BIGSPELL     = 7;
const int ACT_LIMITSPELL   = 9;
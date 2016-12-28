//AlaColor colFinal = 0xA75AF7;
AlaColor colFinal = 0xFE8CFC;
AlaColor colPulse = 0x8949cc;

AlaColor ignite_[2] = { 0x000000, colFinal };
AlaPalette ignite = { 2, ignite_ };

AlaColor pulse_[2] = { colFinal, colPulse };
AlaPalette pulse = { 2, pulse_ };

AlaColor extinguish_[2] = { colFinal, 0x000000 };
AlaPalette extinguish = { 2, extinguish_ };

AlaColor on_[2] = { colFinal, colFinal };
AlaPalette on = { 2, on_ };

AlaColor on__ = 0xFFFFFF;

AlaColor off_[1] = { 0x000000 };
AlaPalette off = { 1, off_ };

AlaColor cast_[4] = { 0x30173c, 0x541f6d, 0x8d3da9, 0xd484e0 };
AlaPalette cast = { 4, cast_ };

AlaSeq seqCast[] = {
  {ALA_FADECOLORSLOOP, 276, 0, cast},
  {ALA_ENDSEQ}
};

int igniteSpeed = 300;

AlaSeq seqIgnite[] =
{
  { ALA_OFF, 0, 50, off },
  { ALA_FADECOLORS, igniteSpeed, igniteSpeed, ignite },
  { ALA_FADECOLORSLOOP, 4000, 0, pulse },
  { ALA_ENDSEQ }
};

AlaSeq seqExtinguish[] =
{
  { ALA_FADECOLORSLOOP, 4000, 50, pulse },
  { ALA_FADECOLORS, igniteSpeed, igniteSpeed, extinguish },
  { ALA_OFF, 0, 0, off },
  { ALA_ENDSEQ }
};

AlaSeq seqNull[] =
{
  { ALA_ON, 0, 0, off},
  { ALA_ENDSEQ }
};

AlaColor bigcast_[2] = { 0x923592, 0xffccff };
AlaPalette bigcast = { 2, bigcast_ };

AlaColor bigspell_[5] = { 0x732005, 0xdb803e, 0xfca567, 0xffdd6a, 0xfef1ac };
AlaPalette bigspell = { 5, bigspell_ };

AlaSeq seqBigcast[] = {
  {ALA_FADECOLORSLOOP, 250, 0, bigcast},
  {ALA_ENDSEQ}
};

AlaColor smallspell_[5] = {0x749fd6, 0x6fb1ff, 0x86e6fd, 0xc3f2ff, 0xe9fbff};
AlaPalette smallspell = { 5, smallspell_ };

AlaSeq seqBigfinish[] = {
  {ALA_SPARKLE, 125, 1000, bigspell},
  //{ALA_FADECOLORS, 300, 300, extinguish },
  //{ALA_OFF, 0, 250, off },
  //{ALA_FADECOLORS, igniteSpeed, igniteSpeed, ignite },
  {ALA_FADECOLORSLOOP, 4000, 0, pulse },
  {ALA_ENDSEQ}
};

AlaSeq seqSmallfinish[] = {
  {ALA_SPARKLE, 125, 1000, smallspell},
  //{ALA_FADECOLORS, 300, 300, extinguish },
  //{ALA_OFF, 0, 250, off },
  //{ALA_FADECOLORS, igniteSpeed, igniteSpeed, ignite },
  {ALA_FADECOLORSLOOP, 4000, 0, pulse },
  {ALA_ENDSEQ}
};

AlaColor limitcast_[3] = { 0x66a2d4, 0x93deff, 0xdbf0fe };
AlaPalette limitcast = { 3, limitcast_ };

AlaColor limitbreak_[5] = { 0xe18069, 0xffe6d1, 0xfeb478, 0x734930, 0x000000 };
AlaPalette limitbreak = { 5, limitbreak_ };

AlaColor limitfire_[2] = { 0x2d0754, 0xfa8425 };
AlaPalette limitfire = { 2, limitfire_ };

AlaSeq seqLimitbreak[] = {
  {ALA_FADECOLORSLOOP, 250, 4000, limitcast},
  {ALA_SPARKLE, 125, 1000, limitbreak},
  {ALA_FADECOLORS, 300, 300, extinguish },
  {ALA_OFF, 0, 0, off },
  {ALA_ENDSEQ}
};

AlaSeq seqLimitcast[] = {
  {ALA_FADECOLORSLOOP, 250, 0, limitcast},
  {ALA_ENDSEQ}
};

AlaSeq seqLimitfinish[] = {
  {ALA_FADECOLORS, 2500, 2500, limitfire},
  {ALA_SPARKLE, 125, 2500, limitbreak},
  {ALA_FADECOLORS, 300, 300, extinguish },
  {ALA_OFF, 0, 0, off },
  {ALA_ENDSEQ}
};


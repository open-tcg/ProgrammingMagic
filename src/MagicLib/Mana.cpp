﻿#include "stdafx.h"
#include "Mana.h"
#include <algorithm>
#include <sstream>

Mana::Mana()
  : X(0)
{
#define SETZEROMANA(_,__,N) BOOST_PP_TUPLE_ELEM(3,0,N) = 0;
  BOOST_PP_SEQ_FOR_EACH(SETZEROMANA,,ALLMANA)
}

void Mana::Read(std::string spec)
{
#define MANACASE(_,__,N) case BOOST_PP_TUPLE_ELEM(3,1,N): ++BOOST_PP_TUPLE_ELEM(3,0,N); break;
  transform(begin(spec), end(spec), begin(spec), ::toupper);
  std::string numeric;
  for (char c : spec)
  {
    if (isdigit(c))
    {
      numeric += c;
    } else
    {
      switch (c)
      {
        BOOST_PP_SEQ_FOR_EACH(MANACASE,,PRIMARYMANA)
      case 'X': ++X; break;

      case '{': case '}': break; // take care of this later
      }
    }
  }

  if (!numeric.empty())
  {
    Colorless = atof(numeric.c_str());
  }
}

Mana::Mana(std::string spec)
  : Mana()
{
  Read(spec);
}

Mana Mana::Parse(std::string spec)
{
  Mana mana;
  mana.Read(spec);
  return mana;
}

bool Mana::EnoughToPay(const Mana& cost)
{
#define CHECKCOST(_,__,N) (BOOST_PP_TUPLE_ELEM(3,0,N) < cost.BOOST_PP_TUPLE_ELEM(3,0,N)) ||
  if (BOOST_PP_SEQ_FOR_EACH(CHECKCOST,,PRIMARYMANA) false) return false;

  return ConvertedManaCost >= cost.ConvertedManaCost;
}

int Mana::GetConvertedManaCost() const
{
#define SUMMANA(_,x,y) x + BOOST_PP_TUPLE_ELEM(3,0,y)
  return BOOST_PP_SEQ_FOLD_LEFT(SUMMANA,,ALLMANA);
}

std::string Mana::ToString() const
{
#define PRINTMANA(_,__,N) \
  for (size_t i = 0; i < BOOST_PP_TUPLE_ELEM(3,0,N); ++i) \
    s << BOOST_PP_TUPLE_ELEM(3,1,N);

  std::ostringstream s{};

  for (size_t i = 0; i < X; i++) s << 'X';
  if (HasColorless) s << Colorless;
  BOOST_PP_SEQ_FOR_EACH(PRINTMANA, , PRIMARYMANA)

  return s.str();
}

std::ostream& operator<<(std::ostream& os, const Mana& mana)
{
  os << mana.ToString();
  return os;
}
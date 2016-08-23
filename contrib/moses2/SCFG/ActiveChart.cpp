#include <boost/foreach.hpp>
#include <boost/functional/hash_fwd.hpp>
#include "ActiveChart.h"
#include "InputPath.h"
#include "Word.h"
#include "Hypothesis.h"
#include "../Vector.h"

using namespace std;

namespace Moses2
{
namespace SCFG
{
SymbolBindElement::SymbolBindElement()
{
}

SymbolBindElement::SymbolBindElement(
    const SCFG::Word *word,
    const Moses2::Hypotheses *hypos)
:word(word)
,hypos(hypos)
{
  assert( (word->isNonTerminal && hypos) || (!word->isNonTerminal && hypos == NULL));
}

size_t hash_value(const SymbolBindElement &obj)
{
  size_t ret = (size_t) obj.hypos;
  boost::hash_combine(ret, obj.word);

  return ret;
}

////////////////////////////////////////////////////////////////////////////
SymbolBind::SymbolBind(MemPool &pool)
:coll(pool)
,numNT(0)
{
}

void SymbolBind::Add(const Range &range, const SCFG::Word &word, const Moses2::Hypotheses *hypos)
{
  SymbolBindElement ele(&word, hypos);
  coll.push_back(ele);

  if (word.isNonTerminal) {
    ++numNT;
  }
}

std::vector<const SymbolBindElement*> SymbolBind::GetNTElements() const
{
  std::vector<const SymbolBindElement*> ret;

  for (size_t i = 0; i < coll.size(); ++i) {
    const SymbolBindElement &ele = coll[i];
    //cerr << "ele=" << ele.word->isNonTerminal << " " << ele.hypos << endl;

    if (ele.word->isNonTerminal) {
      ret.push_back(&ele);
    }
  }

  return ret;
}

std::string SymbolBind::Debug(const System &system) const
{
  stringstream out;
  BOOST_FOREACH(const SymbolBindElement &ele, coll) {
	out << "(";

	if (ele.word->isNonTerminal) {
		const HypothesisBase &hypoBase = *(*ele.hypos)[0];
		const SCFG::Hypothesis &hypo = static_cast<const SCFG::Hypothesis&>(hypoBase);
		out << hypo.GetInputPath().range;
	}

    out << ele.word->Debug(system);
    out << ") ";
  }
  return out.str();
}
////////////////////////////////////////////////////////////////////////////
ActiveChartEntry::ActiveChartEntry(MemPool &pool)
:m_symbolBind(pool)
{
}

////////////////////////////////////////////////////////////////////////////
ActiveChart::ActiveChart(MemPool &pool)
:entries(pool)
{
}

ActiveChart::~ActiveChart()
{

}

}
}


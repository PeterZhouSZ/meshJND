#ifndef GENERICPARAMETRICMODEL_H
#define GENERICPARAMETRICMODEL_H

#include <vector>

template < typename TParam        ,
           typename TIn = double  ,
           typename TOut = double  >
class GenericParametricModel
{

//-----------------------------------------------------------------------------------

public:
  typedef TParam ParameterType;
  typedef TIn    InputType;
  typedef TOut   OutputType;

//-----------------------------------------------------------------------------------

public:
  GenericParametricModel() {}
  GenericParametricModel(const ParameterType& param) : m_param(param) {}

  virtual void ~GenericParametricModel() {}

//-----------------------------------------------------------------------------------

public:
  void setParameters(const ParameterType& param) { m_param = param; }

//-----------------------------------------------------------------------------------

public:
  virtual void compute(const InputType& in, OutputType& out) = 0;

//-----------------------------------------------------------------------------------

public:
  OutputType compute(const InputType& in)
  {
    OutputType out;
    compute(in, out);

    return out;
  }

  void compute(const std::vector< InputType >& in, std::vector< OutputType >& out) const
  {
    out.clear();

    for(const InputType i : in)
      out.push_back( compute(i) );
  }

  OutputType operator()(const InputType& in)
  {
    return compute(in);
  }

  void operator()(const InputType& in, OutputType& out)
  {
    out = compute(in);
  }

//-----------------------------------------------------------------------------------

protected:
  ParameterType m_param;
};

#endif //GENERICPARAMETRICMODEL_H

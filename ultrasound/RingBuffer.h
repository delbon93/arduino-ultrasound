#pragma once

template<typename TType, int TBufferSize>
class RingBuffer
{
  public:

  RingBuffer(TType initialValue = 0)
  {
    for (auto i = 0; i < TBufferSize; i++) {
      m_Buffer[i] = initialValue;
    }
  }

  void Insert(TType value)
  {
    m_Buffer[m_Index] = value;
    m_Index = (m_Index + 1) % TBufferSize;
  }

  TType GetSum()
  {
    TType sum = 0;
    for (auto i = 0; i < TBufferSize; i++) {
      sum += m_Buffer[i];
    }
    return sum;
  }

  TType GetAverage()
  {
    return GetSum() / TBufferSize;
  }

  private:

  TType m_Buffer[TBufferSize];
  uint32_t m_Index = 0;

};
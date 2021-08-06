/* *****************************************************************************
*  A templated circular buffer.
*  ***************************************************************************** */

#pragma once
#include <Arduino.h>

template <class T, uint16_t MAX_ENTRIES> class CircularBuffer
{
  T m_aBuffer[MAX_ENTRIES];

  // Head is the location of the last value added to the
  // circular buffer. 
  int m_nHead;

  // Track the number of entries stored in the buffer. 
  // Never greater than MAX_ENTRIES;
  int m_nStored;

public:
  CircularBuffer()
    /* Initializes an empty buffer.  */
  {
    m_nHead = 0;
    m_nStored = 0;
  }

  uint8_t ElementSize()
  {
    return sizeof(T);
  }

  bool IsEmpty()
    /* Returns true if no data has been stored in the buffer,
    and false if at least one value has been stored. */
  {
    return m_nStored == 0;
  }

  void Add(const T Data)
    /* Add a new value to the end of the buffer. If the
    buffer has exceed its intrinsic capacity, this will
    overwrite old entries. */
  {
    // Advance the head index to the next location in the
    // circular buffer. If it goes past the end of the buffer
    // the head is moved back to the start. 
    m_nHead = (m_nHead + 1) % MAX_ENTRIES;
    if (m_nStored < MAX_ENTRIES)
      ++m_nStored;

    // Save the data. 
    m_aBuffer[m_nHead] = Data;
  }

  void Add(const T *pData)
    /* Add a new value to the end of the buffer. If the
    buffer has exceed its intrinsic capacity, this will
    overwrite old entries. */
  {
    // Advance the head index to the next location in the
    // circular buffer. If it goes past the end of the buffer
    // the head is moved back to the start. 
    m_nHead = (m_nHead + 1) % MAX_ENTRIES;
    if (m_nStored < MAX_ENTRIES)
      ++m_nStored;

    // Save the data.
    memcpy(&m_aBuffer[m_nHead], pData, sizeof(T));
  }

  T &Head()
    /* A pointer to the last value stored in the buffer.
    Only valid if the buffer is not empty. */
  {
    return *(m_aBuffer + m_nHead);
  }

  T &Tail()
    /* A pointer to the first value stored in the buffer. Only
    valid if the buffer is not empty. */
  {
    return *(m_aBuffer + (m_nHead + MAX_ENTRIES - (m_nStored - 1)) % MAX_ENTRIES);
  }

  void PopTail()
  {
    if (m_nStored > 0)
      --m_nStored;
  }

  void DropRecords(int nRecordsToDrop)
  {
    while (nRecordsToDrop--)
      PopTail();
  }


  void Clear()
    /* Empties the circular buffer. The old data is
    still present, but iterators and count will
    ignore it. */
  {
    m_nStored = 0;
  }

  int CountStored() const
    /* Returns the number of entries that are stored
    in the circular buffer. */
  {
    return m_nStored;
  }

  int MaxSize() const
    /* Returns the maximum number of values that can
    be stored in the circular buffer. */
  {
    return MAX_ENTRIES;
  }

  uint16_t GetTailIndex() const
  {
    return  (m_nHead + MAX_ENTRIES - (m_nStored - 1)) % MAX_ENTRIES;
  }

  void Dump(Print &rDestination)
    /* Writes the current content of the serial buffer to a
    serial stream. The values from teh buffer are written
    in the order stored. */
  {
    for (int i = 0; i < MAX_ENTRIES; ++i)
    {
      rDestination.print(i);
      rDestination.print(": ");
      rDestination.println(m_aBuffer[i]);
    }

    rDestination.print("H: ");
    rDestination.print(m_nHead);
    rDestination.print(", ");
    rDestination.println(m_nStored);
  }


  class ForwardIterator
    /* Class to iterate through the circular buffer starting at the tail
    and ending at the head. */
  {
    // The buffer we iterate through. 
    const CircularBuffer &m_rBuffer;

    // Buffer index at the current position. 
    int m_nCurrent;

    // Number of items that we have read from the buffer. 
    // Iteration ends when this equals the number of stored items.
    int m_nRead;

  public:
    ForwardIterator(const CircularBuffer &rBuffer) : m_rBuffer(rBuffer)
    {
      m_nCurrent = (rBuffer.m_nHead + rBuffer.MaxSize() - (rBuffer.CountStored() - 1)) % rBuffer.MaxSize();
      m_nRead = 0;
    }

    const T &CurrentValue()
      /* Returns the current value. The value cannot
      be modified. Cast away the const iff the function you're
      using doesn't modify the value, but hasn't been declared
      correctly! */
    {
      return *(m_rBuffer.m_aBuffer + m_nCurrent);
    }

    int CurrentIndex()
      /* Returns the current index of the iterator in the buffer. */
    {
      return m_nCurrent;
    }

    int ItemNumber()
    {
      return m_nRead;
    }

    void Next()
      /* Moves the iterator to the next position in the buffer.
      If all the values have been read out, the iterator doesn't
      move. If there are no values in the buffer, the iterator
      doesn't move. */
    {
      if (!AtEnd())
      {
        m_nCurrent = (m_nCurrent + 1) % MAX_ENTRIES;
        ++m_nRead;
      }
    }

    bool AtEnd()
      /* Returns true if all the entries in the buffer have been
      stepped through by the iterator. This won't work if values
      have been added to the buffer since the iterator was created. */
    {
      return m_nRead == m_rBuffer.m_nStored;
    }
  };

  class ReverseIterator
    /* Class to iterate through the circular buffer starting at the head
    and ending at the tail.
    Note that the iterator does not track changes to the buffer. So
    if the buffer is modified after the iterator is created,
    bad things will happen. This includes not returning the
    correct number of elements from the buffer. */
  {
    // The buffer that we iterate through. 
    const CircularBuffer *m_pBuffer;

    // The buffer index of the value at the current position. 
    int m_nCurrent;

    // The number of items that have been read from the buffer.
    // Iteration ends when this reaches the number of items stored. 
    int m_nRead;

  public:
    ReverseIterator(const CircularBuffer &rBuffer) : m_pBuffer(&rBuffer)
      /* Initializes the iterator to begin at the current head
      position in buffer. */
    {
      m_nCurrent = m_pBuffer->m_nHead;
      m_nRead = 0;
    }

    const T &CurrentValue()
      /* Returns pointer to the current value. The value cannot
      be modified. Cast away the const iff the function you're
      using doesn't modify the value, but hasn't been declared
      correctly! */
    {
      return *(m_pBuffer->m_aBuffer + m_nCurrent);
    }

    int CurrentIndex()
      /* Returns the current index of the iterator in the buffer. */
    {
      return m_nCurrent;
    }

    int ItemNumber()
    {
      return m_nRead;
    }

    void Previous()
      /* Moves the iterator to the previous position in the buffer.
      If all the values have been read out, the iterator doesn't
      move. If there are no values in the buffer, the iterator
      doesn't move. */
    {
      if (!AtEnd())
      {
        m_nCurrent = (m_nCurrent - 1 + MAX_ENTRIES) % MAX_ENTRIES;
        ++m_nRead;
      }
    }

    bool AtEnd()
      /* Returns true if all the entries in the buffer have been
      stepped through by the iterator. This won't work if values
      have been added to the buffer since the iterator was created. */
    {
      return m_nRead == m_pBuffer->m_nStored;
    }
  };

};
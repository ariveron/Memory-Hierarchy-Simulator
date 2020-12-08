#ifndef I_TRANSLATION_BUFFER_H
#define I_TRANSLATION_BUFFER_H

struct TLBReturnType
{
  int PhysicalAddress;
  bool TLBHit;
  bool PTHit;
};

class ITranslationBuffer
{
public:
  ITranslationBuffer() = default;
  virtual ~ITranslationBuffer() = default;
  virtual TLBReturnType GetPhysicalAddress(int virtualAddress, bool isWrite) = 0;
  virtual int GetHits() = 0;
  virtual int GetMisses() = 0;
  virtual int GetPageTableReferences() = 0;
};

#endif

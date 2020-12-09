#ifndef I_TRANSLATION_BUFFER_H
#define I_TRANSLATION_BUFFER_H

struct TLBReturnType
{
  long long PhysicalAddress;
  bool TLBHit;
  bool PTHit;
};

class ITranslationBuffer
{
public:
  ITranslationBuffer() = default;
  virtual ~ITranslationBuffer() = default;
  virtual TLBReturnType GetPhysicalAddress(long long virtualAddress, bool isWrite) = 0;
  virtual long long GetHits() = 0;
  virtual long long GetMisses() = 0;
  virtual long long GetPageTableReferences() = 0;
};

#endif



#ifndef __BUG_CATCHER_INC__
#define __BUG_CATCHER_INC__

#include <assert.h>

class BugCatcher {
private:
   unsigned int magic;

public:
   BugCatcher() { magic = 0x1234543; }
   virtual ~BugCatcher() { assertNotDeleted(); magic = 0xdeadbeef; }

   virtual void assertNotDeleted() const {
      assert(magic != 0xdeadbeef);
   }
};

#endif

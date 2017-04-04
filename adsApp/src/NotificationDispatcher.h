/**
   Copyright (c) 2015 Beckhoff Automation GmbH & Co. KG

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
 */

#ifndef _NOTIFICATION_DISPATCHER_H_
#define _NOTIFICATION_DISPATCHER_H_

#include "AdsNotification.h"
#include "AmsHeader.h"
#include "Semaphore.h"

#include <map>
#include <thread>

struct AmsProxy {
    virtual long DeleteNotification(const AmsAddr& amsAddr, uint32_t hNotify, uint32_t tmms, uint16_t port) = 0;
};

struct NotificationDispatcher {
    NotificationDispatcher(AmsProxy& __proxy, VirtualConnection __conn);
    ~NotificationDispatcher();
    bool operator<(const NotificationDispatcher& ref) const;
    void Emplace(uint32_t hNotify, Notification& notification);
    long Erase(uint32_t hNotify, uint32_t tmms);
    inline void Notify() { sem.Post(); }
    void Run();

    const VirtualConnection conn;
    RingBuffer ring;
private:
    std::map<uint32_t, Notification> notifications;
    std::recursive_mutex mutex;
    AmsProxy& proxy;
    Semaphore sem;
    std::thread thread;
};

#endif /* #ifndef _NOTIFICATION_DISPATCHER_H_ */

#include "TurnAsyncTcpSocket.hxx"
#include <boost/bind.hpp>

using namespace std;

namespace reTurn {

TurnAsyncTcpSocket::TurnAsyncTcpSocket(asio::io_service& ioService,
                                       TurnAsyncSocketHandler* turnAsyncSocketHandler,
                                       const asio::ip::address& address, 
                                       unsigned short port,
                                       bool turnFraming) : 
   TurnAsyncSocket(ioService, turnAsyncSocketHandler, address, port, turnFraming),
   AsyncTcpSocketBase(ioService)
{
   mLocalBinding.setTransportType(StunTuple::TCP);
   bind(address, port);
}

void 
TurnAsyncTcpSocket::send(resip::SharedPtr<resip::Data> data)
{
   StunTuple destination(StunTuple::TCP, mConnectedAddress, mConnectedPort);
   AsyncSocketBase::send(destination, data);
}

void 
TurnAsyncTcpSocket::send(unsigned short channel, resip::SharedPtr<resip::Data> data)
{
   StunTuple destination(StunTuple::TCP, mConnectedAddress, mConnectedPort);
   AsyncSocketBase::send(destination, channel, data);
}

void 
TurnAsyncTcpSocket::receive()
{
   if(mTurnFraming)
   {
      AsyncSocketBase::framedReceive();
   }
   else
   {
      AsyncSocketBase::receive();
   }
}

void
TurnAsyncTcpSocket::close()
{
   AsyncSocketBase::close();
}

void 
TurnAsyncTcpSocket::onConnectSuccess()
{
   mTurnAsyncSocketHandler->onConnectSuccess(getSocketDescriptor(), mConnectedAddress, mConnectedPort);
   receive();
}

void 
TurnAsyncTcpSocket::onConnectFailure(const asio::error_code& e)
{
   mTurnAsyncSocketHandler->onConnectFailure(getSocketDescriptor(), e);
}

void 
TurnAsyncTcpSocket::onReceiveSuccess(const asio::ip::address& address, unsigned short port, resip::SharedPtr<resip::Data> data)
{
   handleReceivedData(address, port, data);
   receive();
}

void 
TurnAsyncTcpSocket::onReceiveFailure(const asio::error_code& e)
{
   mTurnAsyncSocketHandler->onReceiveFailure(getSocketDescriptor(), e);
}
 
void 
TurnAsyncTcpSocket::onSendSuccess()
{
   //mTurnAsyncSocketHandler->onSendSuccess(getSocketDescriptor());
}
 
void 
TurnAsyncTcpSocket::onSendFailure(const asio::error_code& e)
{
   mTurnAsyncSocketHandler->onSendFailure(getSocketDescriptor(), e);
}

} // namespace


/* ====================================================================

 Original contribution Copyright (C) 2007 Plantronics, Inc.
 Provided under the terms of the Vovida Software License, Version 2.0.

 The Vovida Software License, Version 2.0 
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution. 
 
 THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 DAMAGE.

 ==================================================================== */

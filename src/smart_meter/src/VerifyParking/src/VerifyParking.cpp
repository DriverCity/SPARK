
#include "VerifyParking.h"
#include <cassert>
#include <vector>
#include "Logger/Logger.h"

namespace spark {



VerifyParking::VerifyParking(): m_cloudService(nullptr)  {}


VerifyParking::~VerifyParking(){

}

ICloudService* VerifyParking::getCloudService() const {
    return this->m_cloudService;
}


void VerifyParking::setCloudService(ICloudService* cloudService){
    this->m_cloudService = cloudService;
}


void VerifyParking::init(ICloudService* cloudService){
    assert(cloudService != nullptr);

    this->m_cloudService = cloudService;

}


spark::IVerifyParking::Result VerifyParking::verify(const ParkingEvent& event)
{
    if( this->m_cloudService->checkConnection() ){

        return static_cast<IVerifyParking::Result>(this->m_cloudService->verifyParkingEvent(event));

    }
    else{

        LOG_DEBUG("Connection to cloud is not available currently. Please try again later.");
        return TIMEOUT;

    }


}

}

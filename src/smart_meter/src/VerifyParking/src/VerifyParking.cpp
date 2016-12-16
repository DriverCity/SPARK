
#include "VerifyParking.h"
#include <cassert>
#include <vector>
#include "Logger/Logger.h"

namespace spark {



VerifyParking::VerifyParking(): m_cloudService(nullptr) , m_db(nullptr) {}


VerifyParking::~VerifyParking(){

}

ICloudService* VerifyParking::getCloudService() const {
    return this->m_cloudService;
}

IParkingDatabase* VerifyParking::getDbPointer() const{
    return this->m_db;
}



void VerifyParking::setCloudService(ICloudService* cloudService){
    this->m_cloudService = cloudService;
}

void VerifyParking::setDbPointer(IParkingDatabase* db){
    this->m_db = db;

}


void VerifyParking::init(ICloudService* cloudService, IParkingDatabase* db){
    assert(cloudService != nullptr);

    assert(db != nullptr);

    this->m_cloudService = cloudService;

    this->m_db = db;

}


spark::IVerifyParking::Result VerifyParking::verify(const ParkingEvent& event){
    if( this->m_cloudService->checkConnection() ){
        std::vector<ParkingEvent> events = this->m_db->getEvents();

        for(ParkingEvent e:events){
            m_cloudService->verifyParkingEvent(e);
        }

        if(m_db->clear()){
        LOG_ERROR("Error clearing database.");
        }

        return static_cast<IVerifyParking::Result>(this->m_cloudService->verifyParkingEvent(event));

    }
    else{

        if(m_db->isValid()){
            m_db->addEvent(event);
        }else{
            LOG_ERROR("Error adding event to database. Database is in invalid state.");
        }

        return TIMEOUT;

    }


}

}

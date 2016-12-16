#ifndef VERIFYPARKING
#define VERIFYPARKING

#include "IVerifyParking.h"
#include "misc/ParkingEvent.h"
#include "CloudService/ICloudService.h"
#include "ParkingDatabase/IParkingDatabase.h"


namespace spark
{


class VerifyParking : public spark::IVerifyParking
{
public:


    /**
     * Default constructor for VerifyParking
     */
    VerifyParking();


    ~VerifyParking();


    /**
     * @brief Initialize VerifyParking.
     * @param cloudService Cloud service.
     * @param db Temporary event database.
     * @pre @p cloudService != nullptr, @p db != nullptr.
     * @post VerifyParking is ready to use.
     */
    void init(ICloudService* cloudService, IParkingDatabase* db);


    /**
     * @brief Register parking event.
     * @param event Parking event.
     * @return OK, if event was stored to cloud. Else an appropriate error result.
     * @pre init() has been called.
     * @post Event is sent to cloud. If cloud is not available. Event is stored to
     *  temporary parking event database.
     */
    Result verify(const ParkingEvent& event);



    ICloudService* getCloudService() const;

    IParkingDatabase* getDbPointer() const;



    void setCloudService(ICloudService* cloudService);

    void setDbPointer(IParkingDatabase* db);




private:
    ICloudService* m_cloudService;
    IParkingDatabase* m_db;


};

}

#endif

angular.module('starter.services', [])

/***************************************************************************************
 * FACTORY
 **************************************************************************************/


/*
 * Description :
 * Factory used for store the Firebase IDs
 */
.factory("Firebase", function() {
  var config = {
    apiKey: "AIzaSyDhjWe4lzQBToEiVRTp98nTp09xKi7LxEM",
    authDomain: "spark2-150308.firebaseapp.com",
    databaseURL: "https://spark2-150308.firebaseio.com",
    storageBucket: "",
    messagingSenderId: "758338198382"
  };
  return firebase.initializeApp(config);
})

/*
 * Description :
 * Factory used to pass info about BLE beacons selected from a state to another
 */
.factory('blePerpheralsService', function() {

  var selectedDeviceId = "";
  var serviceId = "";
  var characteristicId = "";
  var selectedDeviceId = "";

  return {
    getSelectedDeviceId : function() {
      return selectedDeviceId;
    },
    setSelectedDeviceId : function(id) {
      selectedDeviceId = id;
    },
    getSelectedDeviceName : function() {
      return selectedDeviceName;
    },
    setSelectedDeviceName : function(name) {
      selectedDeviceName = name;
    },
    getServiceId : function() {
      return serviceId;
    },
    setServiceId : function(id) {
      serviceId = id;
    },
    getCharacteristicId : function() {
      return characteristicId;
    },
    setCharacteristicId : function(id) {
      characteristicId = id;
    },
    onError: function(reason) {
      alert("ERROR: " + JSON.stringify(reason));
    }
  }
})

/*
 * Description :
 * Factory used to pass info about Vehicle selected from a state to another
 */
.factory('parkCarService', function() {

  var selectedVehicle = "";

  return {
    getSelectedVehicle : function() {
      return selectedVehicle;
    },
    setSelectedVehicle : function(aVehicle) {
      selectedVehicle = aVehicle;
    }
  }
})

/***************************************************************************************
 * SERVICE VEHICLE
 **************************************************************************************/

/*
 * Description :
 * Service used to communicate with www.carqueryapi.com to retrieve information about cars
 */
.service (
  "VehicleSrv",
  function($http, $q) {
    return ({
      getMakes: getMakes,
      getModels: getModels,
      getTrims: getTrims,
      getInfos: getInfos
    });

    /*
     * Description :
     * Get all cars marker
     */
    function getMakes() {
      var request = $http({
        method: "get",
        url: 'http://www.carqueryapi.com/api/0.3/?cmd=getMakes',
        params: {
          action: "get"
        }
      });
      return(request.then(handleSuccess, handleError));
    }

    /*
     * Description :
     * Get all cars model associated to a maker
     */
    function getModels(make) {
      var request = $http({
        method: "get",
        url: 'http://www.carqueryapi.com/api/0.3/?cmd=getModels&make='+make,
        params: {
          action: "get"
        }
      });
      return(request.then(handleSuccess, handleError));
    }

    /*
     * Description :
     * Get all trims associated to a maker and a model
     */
    function getTrims(make,model) {
      var request = $http({
        method: "get",
        url: 'http://www.carqueryapi.com/api/0.3/?&cmd=getTrims&model='+model+'&make='+make,
        params: {
          action: "get"
        }
      });
      return(request.then(handleSuccess, handleError));
    }

    /*
     * Description :
     * Get info about a car based on his unique ID
     */
    function getInfos(id) {
      var request = $http({
        method: "get",
        url: 'http://www.carqueryapi.com/api/0.3/?&cmd=getModel&model='+id,
        params: {
          action: "get"
        }
      });
      return(request.then(handleSuccess, handleError));
    }

    // Transform the successful response
    function handleError(response) {
      if (!angular.isObject(response.data) ||
        !response.data.message
      ) {
        return ($q.reject("An unknown error occurred."));
      }
      // Otherwise, use expected error message
      return ($q.reject(response.data.message));
    }

    function handleSuccess(response) {
      return (response.data);
    }
  }
)

/***************************************************************************************
 * SERVICE CLOUD
 **************************************************************************************/

/*
 * Description :
 * Service used to communicate with the cloud and send several requests
 */
.service (
  "CloudSrv",
  function($http, $q) {
    return ({
      parkEventRequest: parkEventRequest,
    });

    /*
     * Description :
     * Send a new park event request
     */
    function parkEventRequest(aData) {
      var request = $http({
        method: 'post',
        url: 'https://spark2-150308.appspot-preview.com/api/v1.0/parkingEvent/',
        data: aData,
        headers: {'content-Type':'application/json'}
      });
      return(request.then(handleSuccess, handleError));
    }

    // Transform the successful response
    function handleError(response) {
      console.log("error");
      if (!angular.isObject(response.data) ||
        !response.data.message
      ) {
        return ($q.reject("Python API scheme does not match"));
      }
      // Otherwise, use expected error message
      return ($q.reject(response.data.message));
    }

    function handleSuccess(response) {
      return (response.data);
    }
  }
)

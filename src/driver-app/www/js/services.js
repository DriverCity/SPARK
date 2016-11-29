angular.module('starter.services', [])

/***************************************************************************************
 * FACTORY
 **************************************************************************************/

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

.service (
  "VehicleSrv",
  function($http, $q) {
    return ({
      getMakes: getMakes,
      getModels: getModels,
      getTrims: getTrims,
      getInfos: getInfos
    });

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

.service (
  "CloudSrv",
  function($http, $q) {
    return ({
      testRequest: testRequest,
    });

    function testRequest(something) {
      var request = $http({
        method: 'post',
        url: 'https://spark2-150308.appspot-preview.com/api/v1.0/storeParkingEvent',
        data: {
          parkingAreaId: 80,
          registerNumber:"ABC-123",
          parkingType:"PAID",
          parkingDurationInMinutes:30
        },
        headers: {'content-Type':'application/json'}
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

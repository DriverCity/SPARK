angular.module('starter.services', [])

/***************************************************************************************
 * FACTORY
 **************************************************************************************/

.factory("Firebase", function() {
  var config = {
    apiKey: "AIzaSyATBJGyHZppFMC9ISOCstvbTWyybID2oRk",
    authDomain: "spark-145412.firebaseapp.com",
    databaseURL: "https://spark-145412.firebaseio.com",
    storageBucket: "spark-145412.appspot.com",
    messagingSenderId: "203401144566"
  };
  return firebase.initializeApp(config);
})

.factory('blePerpheralsService', function() {

  return {
    onError: function(reason) {
      alert("ERROR: " + JSON.stringify(reason));
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

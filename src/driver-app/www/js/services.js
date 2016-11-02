angular.module('starter.services', [])

/***************************************************************************************
 * FACTORY
 **************************************************************************************/

.factory("Firebase", function() {
  var config = {
    apiKey: "AIzaSyDu0vNBDUArjk1mXycJDiHfILFo3eFMbck",
    authDomain: "personnal-1146c.firebaseapp.com",
    databaseURL: "https://personnal-1146c.firebaseio.com",
    storageBucket: "personnal-1146c.appspot.com",
    messagingSenderId: "906605153702"
  };
  return firebase.initializeApp(config);
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

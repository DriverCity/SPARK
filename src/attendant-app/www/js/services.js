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

.factory('blePerpheralsService', function() {

  return {
    onError: function(reason) {
      alert("ERROR: " + JSON.stringify(reason));
    }
  }
})

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

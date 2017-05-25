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

  return {
    onError: function(reason) {
      alert("ERROR: " + JSON.stringify(reason));
    }
  }
})
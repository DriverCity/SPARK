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

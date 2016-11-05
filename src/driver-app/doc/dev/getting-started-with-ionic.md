# Getting started with Ionic
## Introduction

Ionic is a framework that will allow us to create mobile applications using web technologies. Ionic will base on other frameworks / technologies.

- **AngularJS**, for the front-end and the use of Angular UI Router for managing states.
- **Apache Cordova**, to create a working application on mobile.

Ionic provides a set of directives and services over angular allowing you to quickly create user interface elements.

## Routing

The routing is based on Angular UI Router, the content of the states will be automatically loaded into the ```<nav-ion-view> </nav-ion-view>```.

This is a simple example of routing:

```js
.config(function($stateProvider, $urlRouterProvider) {

  $stateProvider.state('home', {
    url: '/home',
    views: {
      templateUrl: 'templates/home.html',
      controller: 'HomeCtrl'
    }
  })

  $urlRouterProvider.otherwise('/home')
});
```

## The interface

All applications do not have the same interface, but the main structure remains the same (a top bar with the back arrow and the contents below).

```js
<body ng-app="starter">

  <ion-nav-bar class="bar-positive">
      <ion-nav-back-button></ion-nav-back-button>
  </ion-nav-bar>

  <ion-nav-view></ion-nav-view>
</body>
```

# Source

> **Grafikart** on grafikart.fr : [Ionic Framework](https://www.grafikart.fr/tutoriels/cordova/ionic-framework-641)
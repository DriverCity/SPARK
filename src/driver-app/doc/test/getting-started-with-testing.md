# Getting started with testing

## Introduction
Tests are an important part in a project, which insure the code quality.

## Unit Tests
This kind of test allows you to focus on testing individual units in your code.

### Jasmine
Jasmine is the most popular framework for testing Angular apps. It provides functions to define what we are going to test: ```describe()```, what we expect to happen: ```it()```, and how we check that it was successful or not: ```expect()```.

## UI Tests / End-To-End Tests
UI tests are a special kind of Integration Test and allow you to test the User Interface of your app through a browser. You can choose to mock the backend, so you can focus on only testing the frontend.

### Protractor
Protractor is an end-to-end test framework for AngularJS applications. Protractor runs tests against your application running in a real browser, interacting with it as a user would.

## Sources

> **Ashteya Biharisingh** on gonehybrid.com : [How To Write Automated Tests For Your Ionic App - Part 1](http://gonehybrid.com/how-to-write-automated-tests-for-your-ionic-app-part-1)
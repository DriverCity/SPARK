#Spark meeting notes 27.10.2016

##Decisions about the driver and attendant prototypes presented in the meeting
* Driver should be able to set the time until when they will stay
* Should take into account paid time limited parking (limit the time the driver can set)
* in paid parking context theattendant will choose the are where theyare checking for example from a list of areas (assuming we can get the data about the areas)
* in parking disc context the attendant will configure the time that cars are allowed to stay in that area manually.
* attendant scans and sees a list of cars (register numbers) nearby and indication if there is violation or no.
* ideas: the attendant can hide the ones they have checked, might be necessary to clear the cache at some point, 
scan/stop scan may be useful

##Technical decisions
* Might be no need for Kerberos, TLS may be enought
* need to make sure that also the drives has something they can use to prove they have paid (receipt/store info to meter/etc)
* Beacon broadcasts only register number

##Physical deliverables of the project to customer:
* code (on github)
* code documentation + some kind of manual

##Discussion about the project plan
* Consider adding: Milestones, all deadlines, dependencies of stories, explain smart meter, check the use of product/prototype

##To do:
* add problem list and prototypoes to github

JsonEasy::Template::Details
===========

This are Template engine details. User should not add specializations for any of this classes

container_assist.hpp
--------------
This are helper classes to hide choise between Container implementation for JsonObject and JsonArray

handler.hpp
--------------
This is template Handler for parser. It desides how to create subhandlers (based on user type) and chooses
Container specialization to call.

start_handler.hpp
--------------
This is special top-level handler, that can only parse objects or arrays depending on User value type
It also moves all parsed data to the object provided by user

sub_handler.hpp
--------------
Determinates how to create SubHandlers for subtypes.
It also can choose not to create SubHandlers if submaps, subarrays not supported for this type 
if there is no Container specialization for it
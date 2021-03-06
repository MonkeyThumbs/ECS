# SECS 1.1
A light weight simple entity component system written in C++

<h2>What is an Entity Component System</h2>
<a href="https://en.wikipedia.org/wiki/Entity%E2%80%93component%E2%80%93system">Entity component systems</a> (ECS) are most commonly used in game development to allow for greater flexibility when defining what constitutes an entity. On the surface an <i>entity</i> is an aggregate of components, but, in this instance, is simply a unique id use to query the component storage. This allows the behavior of an entity to be changed at runtime by adding or removing components. The <i>components</i> store the data of one aspect of the object. They contain no logic of their own, and are only manipulated by their relative systems. The <i>systems</i> runs continuously and manipulate the components assigned to them. A system may control a single component type or may oversee multiple types, comparing their states and act accordingly.

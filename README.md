# ECS 1.0
A light weight entity component system written in C++

<h2>What is an Entity Component System</h2>
<a href="https://en.wikipedia.org/wiki/Entity%E2%80%93component%E2%80%93system">Entity component systems</a> (ECS) are most commonly used in game development to allow for greater flexibility when defining what constitutes an entity. On the surface an <i>entity</i> is an aggregate of components, but, in this instance, is simply a unique id use to query the component storage. This allows the behavior of an entity to be changed at runtime by adding or removing components. The <i>components</i> store the data of one aspect of the object. They contain no logic of their own, and are only manipulated by their relative systems. The <i>systems</i> runs continuously and manipulate the components assigned to them. A system may control a single component type or may oversee multiple types, comparing their states and act accordingly.

<h2>Why Did I Write It?</h2>
I found myself wanting to create larger and more complex games, and was beginning to feel constrained by standard OOP techniques. I needed a way to break free from the limitations of inheritance. So, I began to research design patterns, but could not find one that fit my needs. That is, until I ran across <a href="http://t-machine.org/index.php/2007/09/03/entity-systems-are-the-future-of-mmog-development-part-1/">an article</a> that piqued my interest. It reminded me of an idea I'd had years ago when wrestling with this same issue. I immediately began to scour the internet for any and all related materials. I found a few examples, but most where still embedding the logic with components or overly complex and bloated. I wanted something fast, light, and cache friendly; something as stated in the article. It was then that I decided to push the concept forward and culled all I had learned in my searches together and write my own.


<h3>What's Next</h3>
I planned to integrate an event/listener pattern. Although, I already have a branch with this implemented. I've found little to no need for it with my current framework as I can simply have a system lookup multiple components and act according to their states -thus eliminating the need for the middleman, but may include it if I or others find the need for said feature.

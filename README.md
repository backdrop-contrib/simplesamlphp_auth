simpleSAMLphp Authentication
======================

The simplesamlphp_auth module makes it possible for Backdrop CMS to support SAML
for authentication of users. The module will auto-provision user accounts into
Backdrop CMS if you want it to. It can also dynamically assign Backdrop CMS
roles based on identity attribute values.


Requirements
------------

You must have SimpleSAMLphp installed and configured as a working service
point (SP) as the module uses your local SimpleSAMLphp SP for the SAML
support. For more information on installing and configuring SimpleSAMLphp as
an SP visit: http://www.simplesamlphp.org.

IMPORTANT: Your SP must be configured to use something other than phpsession
for session storage (in config/config.php set store.type => 'memcache'
or 'sql').

To use memcache session handling you must have memcached installed on your
server and PHP must have the memcache extension. For more information on
installing the memcache extension for PHP visit:
http://www.php.net/manual/en/memcache.installation.php

If you are on a shared host or a machine that you cannot install memcache on
then consider using the sql handler (store.type => 'sql').

Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.


Documentation
-------------

The configuration of the module is fairly straight forward. You will need to
know the names of the attributes that your SP will be making available to the
module in order to map them into Backdrop CMS.


Issues
------

Bugs and Feature requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/simplesamlphp_auth/issues.


Current Maintainers
-------------------

- [Joel Steidl](https://github.com/joelsteidl).
- Seeking additional maintainers.

Credits
-------

- Ported to Backdrop CMS by [Joel Steidl](https://github.com/joelsteidl).
- Originally written for Drupal by
  Steve Moitozo [geekwisdom](http://drupal.org/user/1662)

License
-------

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.

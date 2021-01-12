

## Syntax

- one instruction per line
- case sensitive
- **#** - one line comment
- __#*__ ... __*#__ - multi-lines (can be nested). Must be the 1st instruction of a line.

### types

- **int** : integer (usually 32 bits)
- **signed** : signed integer (usually 32 bits)
- **float** : as the name said
- **string** : character's string
- **void** : for function only, return nothing
- **ref** : reference to a variable (function argument only)

### function definition

*type* *name* ( *args* )
	*type* *var_name*
	...
	*code*
**end**

Example :

``
void my_fync( int arg1, string arg2)
	print "got "; arg1, "and "; arg2
end
``

### instructions

#### printing

**print** *args*
where *args* can be separated by 
	- **;** object are appened 

#### compilation time only

- **NEED** "*name*" - request for module *name*
In the tokenized file, following informations are added
	- *version* (uint8) **must** match the requested one (so *version* has to be bumped  every time the API is changing, for example, when arguments changes)
	- *sub* (uint8) : client has to request the minimum subversion it is looking for. It's actually the number of functions in the module.

## Compilation

- **code** 32k
- **strings** 32k
- **globalvar**

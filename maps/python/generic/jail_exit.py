## @file
## Jail manager portal, used for the /arrest command.

from Atrinik import *
from Jail import Jail

jail = Jail(me)

def main():
    jail.jail(activator, 300, False)

main()

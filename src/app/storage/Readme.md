
Ok, I Need a unified theory of persisting this stuff.

For `data`, I've got 
- nodes
- tags
- contributors

And I need to store both the `snapshot`, and the `current`.
Which means, update them both appropriately. Updating the snapshot on
commit / rebase.
And updating the snapshot on change / rebase.

Then I have history items.
- unsynced
- synced

/technically/, I don't need clients (sharedworker) to store the `synced` data. _but_, I do want full replication, so I'll hand on to it.

Anyway, so we track synced & unsynced. I need a way to persist things orderedly.
Which probably means having a history thing that knows to persist.
So I'll switch history to a record of functions.

And then I'll be able to persist. I think.

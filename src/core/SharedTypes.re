/* TODO move to another file */

module Event = {
  module View = {
    type event =
      | Node(Data.Node.id)
      /* single-view-specific */
      | NodeStatus(int, Data.Node.id)
      | Root(int)
      | Mode(int);
  };
  type t =
    | View(View.event)
    | Node(Data.Node.id)
    | Tag(Data.Tag.id)
    | MetaData(string)
    | Contributor(string)
    | User
    | Update
    | Root;
};

/* type edit('contents, 'prefix) =
   | DeleteNode(Node.id)
   | Node(Node.t('contents, 'prefix))
   | NodeChildren(Node.id, list(Node.id))
   | NodeCollapsed(Node.id, bool)
   | Create(Node.t('contents, 'prefix))
   | View(view); */

type dropPos =
  | Above
  | Below
  | Child
  | ChildAbove
  | At(int)
  | End;

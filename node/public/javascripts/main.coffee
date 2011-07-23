Card = (name, kind) ->
  @name = name
  @kind = kind
  @group = Math.floor(Math.random()*4)
  @size = Math.floor(Math.random()*3)
  null

viewModel = {
  test: ko.observable("test value")

  phys: new Card(name, "phy") for name in ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"]
  vms: new Card("v " + name, "vm") for name in ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"]


  modes: ["Physical machines", "Virtual machines", "Storage Machines", "Storage volumes"]
  selectedMode: ko.observable("Physical machines")
  selectMode: (mode) ->
    this.selectedMode(mode)
    if(mode == "Physical machines")
      $("#cards").isotope({filter: ".card-kind-phy"})
    else if(mode == "Virtual machines")
      $("#cards").isotope({filter: ".card-kind-vm"})

}

viewModel.cards = ko.observableArray([].concat viewModel.vms, viewModel.phys)

window.viewModel = viewModel

ko.applyBindings viewModel



$('#cards').isotope {
  itemSelector : '.card'
  layoutMode : 'fitRows'
  #  filter: ".card-kind-phy"
  getSortData: {
    group: (elem) -> elem.attr("data-group")
    size: (elem) -> elem.attr("data-size")
  }
}

$("#sortByOriginal").click -> $("#cards").isotope {sortBy: "original-order"}
$("#sortByGroup").click -> $("#cards").isotope {sortBy: "group"}
$("#sortBySize").click -> $("#cards").isotope {sortBy: "size"}

